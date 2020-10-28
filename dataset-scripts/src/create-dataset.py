#!/usr/bin/env python3


import argparse
import glob
import os
import pathlib
import subprocess
import sys
import tempfile
import time
from typing import List, Tuple

from dataset_json_processor import DatasetJSONProcessor

# Sample invocation
# python3 create-dataset-from-jsons.py supervisor --jsons-dir /path/to/phase2/jsons/folder/ \
# --packages-sources-dirs /path/to/sources/dir1 /path/to/sources/dir2 --output-dir ./dataset --workers 5

# TODO
# Use wget instead of python-requests in Allstar repo or configure retries.


def compute_workers_split(full_workload_size: int, total_worker_count: int) -> List[Tuple[int, int]]:
    min_worker_data_size = round(full_workload_size/total_worker_count)
    workload_split = []
    for worker_id in range(1, total_worker_count + 1):
        start_index = (worker_id - 1) * min_worker_data_size
        if worker_id == total_worker_count:
            end_index = full_workload_size
        else:
            end_index = worker_id * min_worker_data_size

        workload_split.append((start_index, end_index))

    return workload_split


def create_argument_parser() -> argparse.ArgumentParser:
    """
    Command line arguments parser
    """

    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers(dest="mode")
    subparsers.required = True
    # Supervisor mode argument parser
    supervisor_subparser = subparsers.add_parser("supervisor")
    supervisor_subparser.add_argument("-jd", "--jsons-dir", type=str, required=True, help="JSON functions data dir")
    supervisor_subparser.add_argument("-psd", "--packages-source-dirs", type=str, nargs="+", required=True,
                                      help="Directories with package sources")
    supervisor_subparser.add_argument("-od", "--output-dir", type=str, required=True, help="Dataset out directory")
    supervisor_subparser.add_argument("--workers", type=int, required=False, help="Total worker count", default=1)
    # Worker mode argument parser
    worker_subparser = subparsers.add_parser("worker")
    worker_subparser.add_argument("-jl", "--jsons-list-file", type=str, required=True, help="File with JSONs to process")
    worker_subparser.add_argument("-psd", "--packages-source-dirs", type=str, nargs="+", required=True,
                                      help="Directories with package sources")
    worker_subparser.add_argument("-od", "--output-dir", type=str, required=True, help="Dataset out directory")
    worker_subparser.add_argument("-lf", "--log-file", type=str, required=True, help="Log file for worker output")
    return parser


def do_supervisor(args: argparse.Namespace) -> None:
    jsons_dir = pathlib.Path(args.jsons_dir).resolve()
    if not os.path.isdir(jsons_dir):
        print(f"{jsons_dir} is not a valid directory.", file=sys.stderr)
        return

    dataset_out_dir = args.output_dir
    packages_sources_dirs = args.packages_source_dirs
    workers_count = args.workers
    worker_program = os.path.realpath(__file__)
    log_dir = pathlib.Path(os.path.dirname(__file__)) / "logs"
    log_dir.mkdir(exist_ok=True)
    json_files = glob.glob(f"{str(jsons_dir)}/*.json")
    print("Computing downloader workload split...", end="", flush=True)
    workers_split = compute_workers_split(len(json_files), workers_count)
    worker_data_dir = tempfile.TemporaryDirectory()
    worker_data_dir_path = pathlib.Path(worker_data_dir.name)
    workers_data = []
    for index, worker_indices in enumerate(workers_split, start=1):
        worker_record = {}
        worker_data_file = worker_data_dir_path / f"worker{index}.txt"
        worker_slice = json_files[worker_indices[0]:worker_indices[1]]
        with worker_data_file.open("w") as ofh:
            ofh.write(os.linesep.join(worker_slice) + os.linesep)

        worker_record["input_file"] = str(worker_data_file)
        worker_record["count"] = len(worker_slice)
        worker_record["log_file"] = str(log_dir / f"worker{index}.log")
        workers_data.append(worker_record)

    print("done.")
    print(f"Downloaders outputs will be logged in folder {str(log_dir)}")
    print(f"Starting {workers_count} downloaders...", end="", flush=True)

    downloader_processes = []
    for data in workers_data:
        downloader_cmd = ["python3", worker_program, "worker", "-jl", data["input_file"], "-od", dataset_out_dir, "-psd"] + \
                          packages_sources_dirs + ["-lf", data["log_file"]]
        p = subprocess.Popen(downloader_cmd)
        downloader_processes.append(p)

    print("done.")
    print("Waiting for downloaders to exit")
    done_downloaders = []
    tick_count = 0
    tick_length = 60  # seconds
    while True:
        time.sleep(tick_length)
        tick_count += 1
        finished_downloaders_indexes = []
        for index, downloader in enumerate(downloader_processes):
            if downloader.poll() is not None:
                finished_downloaders_indexes.append(index)

        print(f"{len(finished_downloaders_indexes)} finished in tick {tick_count}")
        for index in reversed(finished_downloaders_indexes):
            done_downloaders.append(downloader_processes.pop(index))

        if len(downloader_processes) == 0:
            # All downloaders done
            break

        print(f"{len(downloader_processes)} still running")

    print("All downloaders exited.")


def do_worker(args: argparse.Namespace) -> None:
    packages_sources_dirs = args.packages_source_dirs
    dataset_out_dir = args.output_dir
    jsons_list_file = pathlib.Path(args.jsons_list_file).resolve()
    log_file = pathlib.Path(args.log_file).resolve()
    lfh = log_file.open('w')
    with jsons_list_file.open() as ifh:
        worker_slice = list(map(lambda x: x.rstrip(), ifh.readlines()))

    print(f"Processing {len(worker_slice)} packages", file=lfh)
    dc = DatasetJSONProcessor(worker_slice, packages_sources_dirs, dataset_out_dir, lfh)
    dc.create_dataset()
    print(f"Completed processing {len(worker_slice)} packages", file=lfh)
    lfh.close()


def main():
    parser = create_argument_parser()
    args = parser.parse_args()
    packages_sources_dirs = args.packages_source_dirs

    error = False
    for adir in packages_sources_dirs:
        if not os.path.isdir(adir):
            print(f"{adir} is not a valid directory.")
            error = True
    else:
        if error:
            return

    if args.mode == "supervisor":
        do_supervisor(args)
        print("Some packages could be not yet processed. Some might have failed because Allstar repo closed connection.")
        print("Some might have failed because of JSON decoding errors in Allstar package metadata files.")
        print("You will have to manually fix JSON errors. Most will be cases where a list of multiple values should be used.")
        print("Simply fix JSONs and rerun the supervisor again. All processed packages won't be processed again.")
    elif args.mode == "worker":
        do_worker(args)


if __name__ == "__main__":
    main()
