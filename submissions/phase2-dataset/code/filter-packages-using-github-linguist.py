#!/usr/bin/env python3

# Level 2: Classify packages based on if main programming language is C using github-linguist

import argparse
import os
import shutil
from pathlib import Path

import delegator


def are_dependencies_available():
    """
    Check if git and github-linguist are available
    """

    dependencies = ["git", "github-linguist"]
    for dependency in dependencies:
        if shutil.which(dependency) is None:
            print(f"{dependency} is  not installed")
            return False

    return True


def create_argument_parser():
    parser = argparse.ArgumentParser()
    parser.add_argument("-pd", "--packages-dir", type=str, required=True,
                        help="Directory with all extracted package directories to test")
    parser.add_argument("-o", "--output", type=str, required=True, help="File to write dump output to")
    return parser


def is_package_implemented_in_c(package_dir):
    """
    Check if package is mainly implemented in C using github-linguist. Requires creating a git repository.
    """

    os.chdir(package_dir)
    git_commands = ["git init", "git add .", "git commit -m 'blah'"]
    cleanup_git_command = "rm -rf .git"
    for command in git_commands:
        c = delegator.run(command)
        if c.return_code != 0:
            print(f"failed to create git repo")
            delegator.run(cleanup_git_command)
            return "Failed"

    is_c_package = False
    c = delegator.run("github-linguist")
    if c.return_code != 0:
        print(f"github-linguist exited with error code {c.return_code}")
        delegator.run(cleanup_git_command)
        return "Failed"
    else:
        linguist_output = c.out.strip()
        if linguist_output != "":
            try:
                percentages = dict(entry.replace(' ', '').split('%') for entry in linguist_output.split(os.linesep))
            except ValueError:
                print(f"failed to compute language percentage")
                delegator.run(cleanup_git_command)
                return "Failed"

            is_c_package = (percentages[max(percentages.keys(), key=float)] == 'C')

    delegator.run(cleanup_git_command)
    return is_c_package


def main():
    if not are_dependencies_available():
        return

    arg_parser = create_argument_parser()
    args = arg_parser.parse_args()
    extracted_packages_dir = Path(args.packages_dir)
    output_file = Path(args.output).absolute()

    if not extracted_packages_dir.is_dir():
        print(f"{extracted_packages_dir} is not a valid directory.")
        return

    packages_list = next(os.walk(extracted_packages_dir))[1]
    if packages_list == []:
        print(f"No extracted packages found in {extracted_packages_dir}")
        return

    classified_output = {"c": [], "not-c": [], "failed": []}
    total = len(packages_list)
    print(f"Found {total} packages")
    for count, package_dir in enumerate(packages_list, 1):
        print(f"Checking {package_dir}({count}/{total})...", end="", flush=True)
        package_dir_location = extracted_packages_dir / package_dir
        result = is_package_implemented_in_c(package_dir_location)
        if result == "Failed":
            classified_output["failed"].append(package_dir)
        elif result:
            classified_output["c"].append(package_dir)
            print("is c")
        else:
            classified_output["not-c"].append(package_dir)
            print("not c")

    fh = open(output_file, 'w')
    for package in classified_output["c"]:
        fh.write(package + os.linesep)

    fh.close()
    print("Failed to process following packages")
    for package in classified_output["failed"]:
        print(package)


if __name__ == "__main__":
    main()
