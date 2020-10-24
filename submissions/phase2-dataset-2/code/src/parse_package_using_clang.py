#!/usr/bin/env python3

# Clang based parser to extract function names and line numbers from C/C++ software packages/repos

import glob
import itertools
import json
import os
import pathlib
import sys
from typing import Any, Dict, Iterator, List

import clang.cindex

# Type aliases
PATH = str

# Names of directories that commonly contain source files
SOURCE_DIR_NAMES = ["src", "source"]


def find_functions_in_tree(root_node: clang.cindex.Cursor, package_tree_dir: PATH) -> Dict[PATH, List[Dict[str, Any]]]:
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
        if os.path.commonpath([package_tree_dir, root_node.location.file.name]) == os.path.commonpath([package_tree_dir]):
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


def find_functions_in_source_file(package_tree_dir: PATH, source_file: PATH) -> Dict[PATH, List[Dict[str, Any]]]:
    """
    Find functions and their line number range in a source file. Does not validate if source file exists. Also, finds inline
    functions defined in any headers included.

    :param package_tree_dir:    a
    :param source_file:         Full path to the source file to process
    :return                     A dictionary with names of functions and their start and end lines in source file
    """

    index = clang.cindex.Index.create()
    trans_unit = index.parse(source_file)
    cursor = trans_unit.cursor
    return find_functions_in_tree(cursor, package_tree_dir)


def find_source_files(sources_dir: str) -> Iterator[str]:
    """
    Return iterator of all C source files found by recursively searching the specified folder. Naive search based on file
    extension and pattern matching. Searches for both *.c and *.C because apparently some packages use *.C extension

    :param sources_dir:     Directory to search in for source files
    :return                 An iterator of paths of source file relative to sources_dir
    """

    src_files = [glob.iglob(f"{sources_dir}/**/*.c", recursive=True), glob.iglob(f"{sources_dir}/**/*.C", recursive=True)]
    return itertools.chain(*src_files)


def create_argument_parser():
    """
    Create CLI arguments parser
    """

    argparser = argparse.ArgumentParser()
    # Mandatory arguments
    argparser.add_argument("-pt", "--package_tree", type=str, required=True, help="Software package/repo source tree directory")
    argparser.add_argument("-of", "--output_file", type=str, required=True, help="File to dump JSON output to")
    # Optional arguments
    # If package name is not specified, the name of source tree directory will be used
    argparser.add_argument("-pn", "--package-name", type=str, help="A shortname for package")
    # If src_dir is not specified, a list of possible directory names will be tried
    argparser.add_argument("-sd", "--src-dirs", nargs='+', type=str, help="Name of source directory in package/repo source tree")
    return argparser


def main(package_name, src_dirs, output_file, package_tree):
    package_src_dirs = []

    output_file = os.path.abspath(output_file)
    package_tree_dir = os.path.abspath(package_tree)
    if not os.path.isdir(package_tree_dir):
        print(f"{package_tree_dir} is not a valid directory", file=sys.stderr)
        return

    if not package_name:
        package_name = os.path.basename(package_tree_dir)

    if src_dirs:
        for src_dir in src_dirs:
            package_src_dir = os.path.join(package_tree_dir, src_dir)
            if not os.path.isdir(package_src_dir):
                print(f"{src_dir} not found in {package_tree_dir}. Ignoring.", file=sys.stderr)
            else:
                package_src_dirs.append(package_src_dir)
    else:
        for src_dir_name in SOURCE_DIR_NAMES:
            src_dir = os.path.join(package_tree_dir, src_dir_name)
            if os.path.isdir(src_dir):
                package_src_dir = src_dir
                package_src_dirs.append(src_dir)

    source_files = []
    if package_src_dirs == []:
        # Some packages just put source files at package root
        source_files = find_source_files(package_tree_dir)
    else:
        temp_source_files = []
        for src_dir in package_src_dirs:
            temp_source_files.append(find_source_files(src_dir))

        source_files = itertools.chain(*temp_source_files)

    package_functions = {}
    source_files_found = False
    for source_file in source_files:
        source_files_found = True
        functions_data = find_functions_in_source_file(package_tree_dir, source_file)
        # TODO: Deal with duplicates that may occur
        for src_file, src_file_data in functions_data.items():
            if src_file in package_functions:
                for data in src_file_data:
                    if data not in package_functions[src_file]:
                        package_functions[src_file].extend(data)
            else:
                package_functions[src_file] = src_file_data

    if not source_files_found:
        print(f"No C source files found in {package_tree_dir}", file=sys.stderr)
    else:
        fh = open(output_file, 'w')
        json.dump({package_name: package_functions}, fh, indent=4)
        fh.close()
    return


if __name__ == "__main__":
    import argparse
    argparser = create_argument_parser()
    args = argparser.parse_args()
    main(args.package_name, args.src_dirs, args.output_file, args.package_tree)
