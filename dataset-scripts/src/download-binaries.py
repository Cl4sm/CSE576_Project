#!/usr/bin/env python3

# Dataset binaries downloader
# Use after running create-dataset.py
# Sample invocation: python3 download-binaries.py supervisor --dataset-dir <dataset_dir> --downloaders 10
# <dataset_dir> is dataset directory created by create-dataset.py

import argparse
import json
import os
import pathlib
import shutil
import subprocess
import tempfile
import time
from typing import List, Tuple

import requests


def compute_downloaders_split(full_workload_size: int, total_worker_count: int) -> List[Tuple[int, int]]:
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
    supervisor_subparser.add_argument("-dd", "--dataset_dir", type=str, required=True, help="Dataset directory")
    supervisor_subparser.add_argument("-dw", "--downloaders", type=int, required=True, help="Number of downloaders to run")
    supervisor_subparser.add_argument("-ld", "--log-dir", type=str, required=False, help="Downloader logs directory", default="logs")
    # Download worker mode argument parser
    downloader_subparser = subparsers.add_parser("downloader")
    downloader_subparser.add_argument("-i", "--input", type=str, required=True, help="File with download details")
    downloader_subparser.add_argument("-lf", "--log-file", type=str, required=True, help="Log file for downloader output")
    downloader_subparser.add_argument("-c", "--count", type=int, required=False, help="Count of files to download", default=0)
    return parser


def do_requests_download(url: str, out_file: str) -> None:
    r = requests.get(url)
    ofh = open(out_file, 'wb')
    ofh.write(r.content)
    ofh.close()


def do_wget_download(url: str, out_file: str, log_fh) -> None:
    cmd = ["wget", "-c", url, "-O", out_file]
    p = subprocess.Popen(cmd, stdout=log_fh, stderr=log_fh)
    p.wait()


def do_downloader(args: argparse.Namespace) -> None:
    total_count = args.count
    input_file = pathlib.Path(args.input).resolve()
    log_file = pathlib.Path(args.log_file).resolve()
    current_count = 0
    wget_exists = (shutil.which("wget") is not None)

    lfh = log_file.open("w")
    ifh = input_file.open()
    while True:
        aline = ifh.readline()
        if aline == "":
            break
        current_count += 1
        try:
            download_data = json.loads(aline)
        except json.JSONDecodeError:
            print(f"JSON decoding {aline} failed(line {current_count})", file=lfh)
            continue

        out_file = download_data["output_file"]
        url = download_data["url"]
        out_file_dir = os.path.dirname(out_file)
        os.makedirs(out_file_dir, exist_ok=True)
        print(f"Downloading {os.path.basename(out_file)}", end="", file=lfh)
        if total_count != 0:
            print(f"({current_count}/{total_count})", end="", file=lfh)

        if wget_exists:
            print(" using wget", flush=True, file=lfh)
            do_wget_download(url, out_file, lfh)
        else:
            print(" using requests...", end="", flush=True, file=lfh)
            do_requests_download(url, out_file)
            print("done.", file=lfh)

    ifh.close()
    lfh.close()


def do_supervisor(args: argparse.Namespace) -> None:
    dataset_dir = pathlib.Path(args.dataset_dir).resolve()
    if not dataset_dir.is_dir():
        print(f"{dataset_dir} is not a valid directory")
        return

    downloader_program = os.path.realpath(__file__)
    downloaders_count = args.downloaders
    if os.path.isabs(args.log_dir):
        log_dir = pathlib.Path(args.log_dir)
    else:
        log_dir = dataset_dir.parent / args.log_dir

    log_dir.mkdir(exist_ok=True)

    binaries_to_download = []
    for adir in dataset_dir.iterdir():
        package_data_file = adir / "data.json"
        if not package_data_file.is_file():
            continue

        with package_data_file.open() as fh:
            package_data = json.load(fh)

        package_binaries = package_data["binaries"]
        for binaries in package_binaries.values():
            for binary in binaries:
                binary_record = {"url": binary["url"], "output_file": str(adir / binary["dataset_file"])}
                binaries_to_download.append(json.dumps(binary_record) + os.linesep)

    print("Computing downloader workload split...", end="", flush=True)
    downloaders_split = compute_downloaders_split(len(binaries_to_download), downloaders_count)
    downloader_data_dir = tempfile.TemporaryDirectory()
    downloader_data_dir_path = pathlib.Path(downloader_data_dir.name)
    downloaders_data = []
    for index, downloader_indices in enumerate(downloaders_split, start=1):
        downloader_record = {}
        downloader_data_file = downloader_data_dir_path / f"downloader{index}.txt"
        downloader_slice = binaries_to_download[downloader_indices[0]:downloader_indices[1]]
        with downloader_data_file.open("w") as ofh:
            ofh.writelines(downloader_slice)

        downloader_record["input_file"] = str(downloader_data_file)
        downloader_record["count"] = len(downloader_slice)
        downloader_record["log_file"] = str(log_dir / f"downloader{index}.log")
        downloaders_data.append(downloader_record)

    print("done.")
    print(f"Downloaders outputs will be logged in folder {str(log_dir)}")
    print(f"Starting {downloaders_count} downloaders...", end="", flush=True)

    downloader_processes = []
    for data in downloaders_data:
        downloader_cmd = ["python3", downloader_program, "downloader", "-i", data["input_file"], "-lf", data["log_file"],
                          "-c", str(data["count"])]
        p = subprocess.Popen(downloader_cmd)
        downloader_processes.append(p)

    print("done.")
    print("Waiting for downloaders to exit")
    done_downloaders = []
    tick_count = 0
    while True:
        time.sleep(10)
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


def main():
    parser = create_argument_parser()
    args = parser.parse_args()
    if args.mode == "supervisor":
        do_supervisor(args)
    else:
        do_downloader(args)


if __name__ == "__main__":
    main()
