#!/usr/bin/env python3


import argparse
import contextlib
import datetime
import itertools
import json
import os
import pathlib
import shutil
import subprocess
import sys
import tempfile
from typing import Iterable, List, Union

import clang.cindex
import delegator

# If you get an error "couldn't find file libclang.so", create a symlink named libclang.so to the libclang SO file
# installed in some folder on the library search path(eg: /usr/lib/x86_64-linux-gnu)


def build_package(pkg_src_root: pathlib.Path) -> int:
    """
    Verify if package sources exist and build DEB package
    """

    pkg_src_name = find_pkg_src_name(pkg_src_root)

    # Install build dependencies
    returncode = None
    stdout = None
    stderr = None
    print("Installing build dependencies...", end="", flush=True)
    build_dep_cmd = ["apt-get", "build-dep", "-y", pkg_src_name]
    p = subprocess.Popen(build_dep_cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    p.wait()
    if p.returncode != 0:
        print("failed.")
        stdout, stderr = p.communicate()
        print("stdout")
        print(stdout)
        print("stderr")
        print(stderr)
        return p.returncode

    print("done.")
    build_timeout = 60 * 60  # 60 minutes
    print("Building...", end="", flush=True)
    build_cmd = ["bear", "--", "dpkg-buildpackage", "-b", "-uc", "-us"]  # , "-j8"]
    with save_cwd(pkg_src_root):
        try:
            subprocess.check_output(build_cmd, stderr=subprocess.STDOUT, timeout=build_timeout)
        except subprocess.TimeoutExpired:
            print("timeout.")
            return 1
        except subprocess.CalledProcessError as e:
            print("failed.")
            return e.returncode

    print("done.")
    return 0


def create_argument_parser() -> argparse.ArgumentParser:
    """
    Command line arguments parser
    """

    parser = argparse.ArgumentParser()
    parser.add_argument("-bd", "--bin-dir", type=str, required=True, help="Directory to copy binaries to")
    parser.add_argument("-cd", "--compile-db-dir", type=str, required=True, help="Directory to dump compilation DB to")
    parser.add_argument("-ld", "--logs-dir", type=str, required=True, help="Directory to log output to")
    parser.add_argument("-pn", "--package-name", type=str, required=True, help="Package name")
    parser.add_argument("-pad", "--packages-archives-dir", type=str, required=True, help="Directory with all packages' source archives")
    return parser


def disable_opt(pkg_src_root: pathlib.Path) -> int:
    # Add -O0 using DEB_CFLAGS_APPEND. Remove -O1, -O2, -O3 using DEB_CFLAGS_STRIP
    # Replace -O1/-O2/-O3 in build related files.
    sed_cmd = ["sed", '--in-place=.back', 's/-O1/-O0/g; s/-O2/-O0/g; s/-O3/-O0/g'] + find_build_files(pkg_src_root)
    print("Disabling optimizations...", end="", flush=True)
    p = subprocess.Popen(sed_cmd, stdout=subprocess.DEVNULL, stderr=subprocess.PIPE)
    p.wait()
    if p.returncode != 0:
      print("failed. Dumping stderr.")
      _, err = p.communicate()
      print(err)
    else:
      print("success.")

    # Some packages use default flags from dpkg-buildflags so disable there as well.
    os.putenv("DEB_CFLAGS_APPEND", "-O0")
    os.putenv("DEB_CFLAGS_STRIP", "-O1 -O2 -O3")
    return p.returncode


def download_sources(package_name: str, working_dir: pathlib.Path) -> int:
    with save_cwd(working_dir):
        download_src_cmd = ["apt-get", "source", package_name]
        print("Downloading sources...", end="", flush=True)
        p = subprocess.Popen(download_src_cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        p.wait()
        if p.returncode != 0:
            print("failed.")
            out, err = p.communicate()
            print("stdout")
            print(out)
            print("stderr")
            print(err)
        else:
            print("done.")

        return p.returncode


def extract_binaries(adir: pathlib.Path, bin_out_dir: pathlib.Path) -> int:
    print("Extracting binaries...", end="", flush=True)
    debs = adir.glob("*.deb")
    temp_dir = tempfile.TemporaryDirectory()
    for deb in debs:
        cmd = ["dpkg", "-x", str(deb), temp_dir.name]
        p = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        p.wait()
        if p.returncode != 0:
            out, err = p.communicate()
            print("extracting %s failed." % (deb))
            print("Stdout")
            print(out)
            print("Stderr")
            print(err)
            return p.returncode

    cmd = 'find %s -type f -exec file {} \\; | grep -i elf | cut -d ":" -f 1 | egrep -vi "\\.debug$"' % (temp_dir.name)
    c = delegator.run(cmd)
    if c.return_code != 0:
        print("finding executables failed.")
        print("Stdout")
        print(c.out)
        print("Stderr")
        print(c.err)
    else:
        binaries = c.out.strip().split(os.linesep)
        for binary in binaries:
            shutil.copy2(binary, str(bin_out_dir))

        print("done.")

    return c.return_code


def find_build_files(pkg_src_root: pathlib.Path) -> List[str]:
    patterns = ["configure", "configure.*", "Makefile", "Makefile.*", "*.Makefile", "makefile", "makefile.*",
                "*.makefile", "*.m4", "debian/rules"]

    build_files = []
    for pattern in patterns:
        files_matching_pattern = filter(lambda x: x.is_file(), pkg_src_root.rglob(pattern))
        build_files.extend(list(map(str, files_matching_pattern)))

    return build_files


def find_functions_in_tree(root_node: clang.cindex.Cursor, package_tree_dir: pathlib.Path):
    """
    Find functions in AST (sub-)tree rooted at root_node

    :param root_node:           Root of the AST (sub-)tree being processed
    :param package_tree_dir:    Root folder of the source tree
    :return                     A dictionary of source files processed, functions present in them and start and end lines of
                                functions in source file
    """

    functions = {}
    if root_node.kind == clang.cindex.CursorKind.FUNCTION_DECL:
        # We are processing a function. Check if it's in the package or outside(eg: system header)
        if os.path.commonprefix([str(package_tree_dir), root_node.location.file.name]) == str(package_tree_dir):
            # Extract function name, path relative to location of source root, start and end line of function
            # TODO: Maybe we should check if the start and end line differ; if not, maybe it's just a declaration and no function
            # body is present?
            fn_name = root_node.spelling
            fn_src_file = str(pathlib.PurePath(root_node.location.file.name).relative_to(package_tree_dir))
            fn_start_line = root_node.extent.start.line
            fn_end_line = root_node.extent.end.line
            fn_record = {"name": fn_name, "start": fn_start_line, "end": fn_end_line}
            if fn_end_line != fn_start_line:
                if fn_src_file in functions:
                    functions[fn_src_file].append(fn_record)
                else:
                    functions[fn_src_file] = [fn_record]

    for child in root_node.get_children():
        child_functions = find_functions_in_tree(child, package_tree_dir)
        for child_src_file, child_src_file_fns in child_functions.items():
            if child_src_file in functions:
                functions[child_src_file].extend(child_src_file_fns)
            else:
                functions[child_src_file] = child_src_file_fns

    return functions


def find_functions_in_source_file(package_tree_dir: pathlib.Path, source_file: pathlib.Path):
    """
    Find functions and their line number range in a source file. Does not validate if source file exists. Also, finds inline
    functions defined in any headers included.

    :param package_tree_dir:    a
    :param source_file:         Full path to the source file to process
    :return                     A dictionary with names of functions and their start and end lines in source file
    """

    index = clang.cindex.Index.create()
    try:
        trans_unit = index.parse(str(source_file))
        cursor = trans_unit.cursor
        return find_functions_in_tree(cursor, package_tree_dir)
    except clang.cindex.TranslationUnitLoadError:
        return {}


def find_headers_and_source_files(sources_dir: pathlib.Path) -> Iterable[pathlib.Path]:
    """
    Return iterator of all C header and source files found by recursively searching the specified folder. Naive search based on file
    extension and pattern matching. Searches for both *.c and *.C because apparently some packages use *.C extension

    :param sources_dir:     Directory to search in for source files. pathlib.Path.
    :return                 An iterator of paths of source file relative to sources_dir.
    """

    src_files = [sources_dir.rglob("*.c"), sources_dir.rglob("*.C"), sources_dir.rglob("*.H"), sources_dir.rglob("*.h")]
    return itertools.chain(*src_files)


def find_pkg_src_name(pkg_src_root: pathlib.Path) -> str:
    pkg_debian_control_file = pkg_src_root / "debian" / "control"
    with pkg_debian_control_file.open('rb') as fh:
        while True:
            aline = fh.readline().strip()
            if aline.startswith(b"Source: "):
                return str(aline.split(b" ")[1], "utf-8")


def find_pkg_root(working_dir: pathlib.Path) -> Union[pathlib.Path, None]:
    for afile in working_dir.iterdir():
        if afile.is_dir():
            if afile / "debian" / "control":
                return afile


def insert_noinline_attribute(pkg_src_root: pathlib.Path) -> None:
    headers_src_files = find_headers_and_source_files(pkg_src_root)
    insert_locations = {}
    for afile in headers_src_files:
        function_details = find_functions_in_source_file(pkg_src_root, afile)
        for src_file, functions in function_details.items():
            for f in functions:
                if src_file in insert_locations:
                    insert_locations[src_file].add(f["start"])
                else:
                    insert_locations[src_file] = set([f["start"]])

    out_str = b"__attribute__ ((noinline)) "
    for src_file, line_nos in insert_locations.items():
        src_file_path = pkg_src_root / src_file
        with src_file_path.open("rb") as ifh:
            input_lines = ifh.readlines()

        with src_file_path.open('wb') as ofh:
            for ind, line in enumerate(input_lines, start=1):
                if ind in line_nos:
                    ofh.write(out_str)

                ofh.write(line)


def remove_inline_keyword(pkg_src_root: pathlib.Path) -> int:
    headers_src_files = find_headers_and_source_files(pkg_src_root)
    sed_cmd = ["sed", '--in-place=.back', 's/ inline / /g']
    print("Replacing all inline keywords...", end="", flush=True)
    p = subprocess.Popen(sed_cmd + list(map(str, headers_src_files)), stdout=subprocess.DEVNULL, stderr=subprocess.PIPE)
    p.wait()
    if p.returncode != 0:
      _, err = p.communicate()
      print("failed. Dumping stderr.")
      print(err)
    else:
      print("done.")

    return p.returncode


def save_compile_db(pkg_src_root: pathlib.Path, compile_db_out_dir: pathlib.Path) -> None:
    print("Saving compilation DB...", end="", flush=True)
    compile_db_file = pkg_src_root / "compile_commands.json"
    fh = compile_db_file.open()
    compile_db = json.load(fh)
    fh.close()
    out_compile_db = {"compile-db": compile_db, "src_root": str(pkg_src_root)}
    pkg_src_name = find_pkg_src_name(pkg_src_root)
    compile_db_out_file = compile_db_out_dir / (pkg_src_name + ".json")
    with compile_db_out_file.open('w') as fh:
        json.dump(out_compile_db, fh, indent=4)

    print("done.")


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

    pkg_name = args.package_name
    packages_archives_dir = pathlib.Path(args.packages_archives_dir).absolute()
    pkg_working_dir = packages_archives_dir / pkg_name
    os.makedirs(str(pkg_working_dir), exist_ok=True)

    # Python 3.4 compatibility: pathlib doesn't support exist_ok
    compile_db_out_dir = pathlib.Path(args.compile_db_dir).absolute()
    os.makedirs(str(compile_db_out_dir), exist_ok=True)

    bin_dir = pathlib.Path(args.bin_dir).absolute()
    os.makedirs(str(bin_dir), exist_ok=True)

    logs_dir = pathlib.Path(args.logs_dir).absolute()
    os.makedirs(str(logs_dir), exist_ok=True)

    log_file = logs_dir / (pkg_name + ".log")
    print("Log file: %s" % (str(log_file)))

    start = datetime.datetime.now()
    print("Start build of %s at %s...." % (pkg_name, start.strftime("%a %b %d %Y %H:%M:%S")), end="", flush=True)
    old_stdout = sys.stdout
    sys.stdout = log_file.open('w')

    retcode = download_sources(pkg_name, pkg_working_dir)
    pkg_src_root = find_pkg_root(pkg_working_dir)
    if not pkg_src_root:
        print("Unable to find extracted source directory for package %s" % (pkg_name))
        sys.stdout.close()
        sys.stdout = old_stdout
        end = datetime.datetime.now()
        print("failed at %s(%s)." % (end.strftime("%a %b %d %Y %H:%M:%S"), (end - start)))
        sys.exit(1)

    retcode = remove_inline_keyword(pkg_src_root)
    if retcode != 0:
        sys.stdout.close()
        sys.stdout = old_stdout
        end = datetime.datetime.now()
        print("failed at %s(%s)." % (end.strftime("%a %b %d %Y %H:%M:%S"), (end - start)))
        sys.exit(retcode)

    insert_noinline_attribute(pkg_src_root)

    retcode = disable_opt(pkg_src_root)
    if retcode != 0:
        sys.stdout.close()
        sys.stdout = old_stdout
        end = datetime.datetime.now()
        print("failed at %s(%s)." % (end.strftime("%a %b %d %Y %H:%M:%S"), (end - start)))
        sys.exit(retcode)

    retcode = build_package(pkg_src_root)
    if retcode != 0:
        sys.stdout.close()
        sys.stdout = old_stdout
        end = datetime.datetime.now()
        print("failed at %s(%s)." % (end.strftime("%a %b %d %Y %H:%M:%S"), (end - start)))
        sys.exit(retcode)

    save_compile_db(pkg_src_root, compile_db_out_dir)

    binaries_out_dir = bin_dir / pkg_name
    os.makedirs(str(binaries_out_dir), exist_ok=True)
    retcode = extract_binaries(pkg_src_root.parent, binaries_out_dir)
    sys.stdout.close()
    sys.stdout = old_stdout
    end = datetime.datetime.now()
    if retcode != 0:
        print("failed at %s(%s)." % (end.strftime("%a %b %d %Y %H:%M:%S"), (end - start)))
        sys.exit(retcode)
    else:
        print("done at %s(%s)." % (end.strftime("%a %b %d %Y %H:%M:%S"), (end - start)))


if __name__ == "__main__":
    main()
