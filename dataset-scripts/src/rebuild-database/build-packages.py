#!/usr/bin/env python3


import argparse
import json
import os
import pathlib
import subprocess


def create_argument_parser() -> argparse.ArgumentParser:
    """
    Command line arguments parser
    """

    parser = argparse.ArgumentParser()
    parser.add_argument("-pl", "--package-list", type=str, required=True, help="File with list of packages to build")
    parser.add_argument("-pad", "--packages-archives-dir", type=str, required=True, help="Packages archives top level dir")
    parser.add_argument("-od", "--output-dir", type=str, required=True, help="Output directory")
    parser.add_argument("-rid", "--runner-id", type=int, required=True, help="Runner ID")
    return parser


def main():
    parser = create_argument_parser()
    args = parser.parse_args()
    base_dir = pathlib.Path(os.path.realpath(__file__)).parent

    package_list = os.path.abspath(args.package_list)
    if not os.path.isfile(package_list):
        print("%s is not a valid file" % (package_list))
        return

    packages_archives_dir = pathlib.Path(args.packages_archives_dir).resolve()
    if not packages_archives_dir.is_dir():
        print("%s is not a valid directory." % (str(packages_archives_dir)))
        return

    output_dir = pathlib.Path(args.output_dir).absolute()
    os.makedirs(str(output_dir), exist_ok=True)

    compile_db_dir = output_dir / "compile-db"
    binaries_dir = output_dir / "binaries"
    logs_dir = output_dir / "logs"

    runner_id = args.runner_id
    runner_output_dir = output_dir / "runners" / ("runner" + str(runner_id))
    os.makedirs(str(runner_output_dir), exist_ok=True)
    runner_log_file = runner_output_dir / "output.log"
    runner_status_file = runner_output_dir / "status.json"
    if runner_status_file.is_file():
        with runner_status_file.open() as runner_status_fh:
            runner_done = json.load(runner_status_fh)
    else:
        runner_done = []

    packages = list(map(lambda x: x.strip(), open(package_list, 'r').readlines()))

    package_build_script = base_dir / "remove-inline-and-build-package.py"
    cmd = ["python3", str(package_build_script), "--bin-dir", str(binaries_dir), "--compile-db-dir", str(compile_db_dir),
           "--logs-dir", str(logs_dir), "--packages-archives-dir", str(packages_archives_dir), "--package-name"]

    runner_log_fh = runner_log_file.open("w")
    for package in packages:
        if package in runner_done:
            continue

        print("Building %s..." % (package), end="", flush=True)
        p = subprocess.Popen(cmd + [package], stdout=runner_log_fh, stderr=runner_log_fh)
        p.wait()
        if p.returncode != 0:
            print("failed.")
        else:
            print("success.")
            runner_done.append(package)
            with runner_status_file.open("w") as runner_status_fh:
                json.dump(runner_done, runner_status_fh, indent=4)


if __name__ == "__main__":
    main()
