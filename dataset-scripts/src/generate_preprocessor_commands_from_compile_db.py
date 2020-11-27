#!/usr/bin/env python3


import argparse
import contextlib
import copy
import json
import os
import pathlib
import shlex
import subprocess
import sys
from typing import Dict, List, Union


def convert_to_preprocess_db(compile_db: List[Dict[str, str]]) -> Union[List[Dict[str, str]], None]:
    """
    Convert compile DB to preprocess DB. Removes -c and -o <arg>
    """

    valid_compilers = ['/usr/bin/gcc', '/usr/bin/cc', 'cc', 'gcc', 'i686-w64-mingw32-gcc', 'x86_64-linux-gnu-gcc']
    preprocessor = "cpp"
    preprocess_db = []
    for compile_cmd_entry in compile_db:
        if not os.path.isfile(compile_cmd_entry["file"]) or not os.path.isdir(compile_cmd_entry["directory"]):
            # Don't process entries of files not in the source tree
            continue
        compile_cmd = fix_quotes_in_command(compile_cmd_entry["command"])
        preprocess_cmd = None
        for compiler in valid_compilers:
            if compile_cmd.startswith(compiler):
                preprocess_cmd = compile_cmd.replace(compiler, preprocessor, 1)
                break

        if preprocess_cmd is None:
            # print("..ignoring compiler %s..." % (compiler), end="", flush=True)
            continue

        # Very hacky way to remove -c but it does mostly work fine except in some cases(see below)
        preprocess_cmd = preprocess_cmd.replace(" -c ", " ")

        # Removing -o is more tricky since it has an argument: we use shlex to find the string to replace but not use
        # shlex for actual replacement because working with shlex when there are double quotes in the command is a pain.
        try:
            preprocess_cmd_list = shlex.split(preprocess_cmd)
        except ValueError:
            print("failed to convert compile DB to preprocess DB. shlex error.")
            return None

        if "-o" in preprocess_cmd_list:
            o_index = preprocess_cmd_list.index("-o")
            string_to_replace = preprocess_cmd_list[o_index] + ' ' + preprocess_cmd_list[o_index + 1]
            preprocess_cmd = preprocess_cmd.replace(string_to_replace, '')

        # Sometimes "-c" is at end of command and that's not caught by above filter
        if "-c" in preprocess_cmd_list:
            if preprocess_cmd_list[-1] != "-c":
                print("-c search-and-replace remove failed but shouldn't have.")
                return None

            # Remove just " -c" from end of string
            preprocess_cmd = preprocess_cmd[:-3]

        # Escaping some stuff
        if "(" in preprocess_cmd:
            preprocess_cmd = preprocess_cmd.replace("(", "\\(")

        if ")" in preprocess_cmd:
            preprocess_cmd = preprocess_cmd.replace(")", "\\)")

        if "<" in preprocess_cmd:
            preprocess_cmd = preprocess_cmd.replace("<", "\\<")

        if ">" in preprocess_cmd:
            preprocess_cmd = preprocess_cmd.replace(">", "\\>")

        # Some commands compile multiple files at a time but preprocessor doesn't like that so we need to create
        # entries for all files
        preprocess_cmd_list = shlex.split(preprocess_cmd)
        with save_cwd(compile_cmd_entry["directory"]):
            curr_file_name = os.path.basename(compile_cmd_entry["file"])
            files_in_cmd = []
            source_files_in_cmd = []
            # We assume the first file in command is the source file being processed and look for all files from there
            # point. There are some cases where this doesn't work: fix those failures manually.
            for token_index, token in enumerate(preprocess_cmd_list):
                if curr_file_name in token:
                    start_index = token_index + 1
                    curr_file_name = token
                    break
            else:
                print("failed to find %s in compile command. that's weird." % (curr_file_name))
                return None

            for token in preprocess_cmd_list[start_index:]:
                if os.path.isfile(token):
                    files_in_cmd.append(token)
                    if token.endswith(".c"):
                        source_files_in_cmd.append(token)

            if len(source_files_in_cmd) > 1:
                for afile in files_in_cmd:
                    preprocess_cmd = preprocess_cmd.replace(afile, '')

                # We mark insert position using the place where current file's name is
                preprocess_cmd = preprocess_cmd.replace(curr_file_name, "{}")

                source_files_in_cmd.insert(0, curr_file_name)
                for afile in source_files_in_cmd:
                    tmp_entry = copy.deepcopy(compile_cmd_entry)
                    tmp_entry["file"] = tmp_entry["file"].replace(curr_file_name, afile)
                    tmp_entry["command"] = preprocess_cmd.format(afile)
                    preprocess_db.append(tmp_entry)
            else:
                tmp_entry = copy.deepcopy(compile_cmd_entry)
                tmp_entry["command"] = preprocess_cmd
                preprocess_db.append(tmp_entry)

    return preprocess_db


def create_argument_parser() -> argparse.ArgumentParser:
    """
    Command line arguments parser
    """

    parser = argparse.ArgumentParser()
    parser.add_argument("-pn", "--package-name", type=str, required=True, help="Package name")
    parser.add_argument("-cdd", "--compile-db-dir", type=str, required=True, help="Compile DB dir")
    parser.add_argument("-pad", "--packages-archives-dir", type=str, required=True, help="Packages archives top level dir")
    parser.add_argument("-sd", "--success-dir", type=str, required=True, help="Directory to dump generated preprocessor commands")
    parser.add_argument("-fd", "--failed-dir", type=str, required=True, help="Directory to dump data about failed commands")
    return parser


def find_pkg_root(working_dir: pathlib.Path) -> Union[pathlib.Path, None]:
    for afile in working_dir.iterdir():
        if afile.is_dir():
            if afile / "debian" / "control":
                return afile


def fix_quotes_in_command(command: str) -> str:
    tmp_command = list(command.replace('"-D', "-D"))
    fixed_command = ''
    total_len = len(tmp_command)
    for curr_index, char in enumerate(tmp_command):
        if char == '"':
            if tmp_command[curr_index - 1] != '=' and (curr_index + 1) != total_len and tmp_command[curr_index + 1] == '"':
                # This is a case of duplicate double quotes. Ignore this double quote: next one will get added.
                continue

        fixed_command += char

    return fixed_command


def install_build_dependencies(package_name: str) -> int:
    install_cmd = ["apt-get", "build-dep", "-y", package_name]
    retcode = subprocess.call(install_cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    if retcode != 0:
        print("installing build deps failed.")

    return retcode


def load_and_normalize_compile_db(compile_db_file: pathlib.Path, src_root: pathlib.Path) -> List[Dict[str, str]]:
    """
    Normalize package's compile DB with given src_root
    """

    with compile_db_file.open() as fh:
        data = json.load(fh)

    orig_src_root = data["src_root"]
    orig_compile_db = data["compile-db"]
    normalized_compile_db = []
    for orig_cmd in orig_compile_db:
        normalized_cmd = {"command": orig_cmd["command"]}
        normalized_cmd["directory"] = orig_cmd["directory"].replace(orig_src_root, str(src_root))
        normalized_cmd["file"] = orig_cmd["file"].replace(orig_src_root, str(src_root))
        normalized_compile_db.append(normalized_cmd)

    return normalized_compile_db


@contextlib.contextmanager
def save_cwd(path: pathlib.Path):
    """
    Stolen from kylebot
    """

    cwd = os.getcwd()
    try:
        os.chdir(str(path))
        yield
    finally:
        os.chdir(cwd)


def main():
    parser = create_argument_parser()
    args = parser.parse_args()

    package_name = args.package_name
    print("Processing %s..." % (package_name), end="", flush=True)

    compile_db_dir = pathlib.Path(args.compile_db_dir).absolute()
    if not compile_db_dir.is_dir():
        print("%s(compile DB dir) is not a valid directory" % (compile_db_dir))
        return
    else:
        compile_db_dir = compile_db_dir.resolve()

    packages_archives_dir = pathlib.Path(args.packages_archives_dir).absolute()
    if not packages_archives_dir.is_dir():
        print("%s(packages archive dir) is not a valid directory." % (str(packages_archives_dir)))
        return
    else:
        packages_archives_dir = packages_archives_dir.resolve()

    package_src_root = find_pkg_root(packages_archives_dir / package_name)
    if not package_src_root:
        print("failed to find package sources folder")
        return

    package_compile_db = compile_db_dir / (package_name + ".json")
    if not package_compile_db.is_file():
        print("compile DB JSON file not found" % (package_name))
        return

    success_output_dir = pathlib.Path(args.success_dir).absolute()
    os.makedirs(str(success_output_dir), exist_ok=True)
    success_output_dir = success_output_dir.resolve()

    failed_output_dir = pathlib.Path(args.failed_dir).absolute()
    os.makedirs(str(failed_output_dir), exist_ok=True)
    failed_output_dir = failed_output_dir.resolve()

    compile_db = load_and_normalize_compile_db(package_compile_db, package_src_root)
    preprocess_db = convert_to_preprocess_db(compile_db)
    if preprocess_db is None:
        return

    total_cmd_count = len(preprocess_db)
    failed_cmds = []
    success_cmds = []
    if install_build_dependencies(package_name) != 0:
        return

    for preprocess_cmd in preprocess_db:
        if not os.path.isfile(preprocess_cmd["file"]):
            # Some of the configure commands would have also been captured. Ignore them.
            total_cmd_count = total_cmd_count - 1
            continue

        with save_cwd(preprocess_cmd["directory"]):
            # Yes yes shell=True is unsafe. Honestly, it's easier to just shell out than figure out correct parsing
            # of the command to avoid shell=True
            retcode = subprocess.call(preprocess_cmd["command"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, shell=True)
            if retcode != 0:
                failed_cmds.append(preprocess_cmd)
            else:
                success_cmds.append(preprocess_cmd)

    if len(success_cmds) != 0:
        success_commands_output_file = success_output_dir / (package_name + ".json")
        with success_commands_output_file.open('w') as fh:
            json.dump(success_cmds, fh, indent=4)

    if len(success_cmds) == total_cmd_count:
        print("done")
    else:
        print("%s/%s commands failed" % (total_cmd_count - len(success_cmds), total_cmd_count))
        failed_commands_output_file = failed_output_dir / (package_name + ".json")
        with failed_commands_output_file.open("w") as fh:
            json.dump(failed_cmds, fh, indent=4)



if __name__ == "__main__":
    main()
