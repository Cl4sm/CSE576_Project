#!/usr/bin/env python3


import argparse
import itertools
import os
import pathlib
from typing import Dict, Iterator, List, Set

import clang.cindex
import delegator
from pwn import ELF

from parse_package_using_clang import (find_functions_in_source_file,
                                       find_source_files)

# Usage notes
# package_src_root: The root directory of extracted sources downloaded from debian repos
# package_bin_dirs: The directory with binaries in the dataset downloaded using the dataset downloader. It will be
# recursively search for binaries. Multiple directories can also be specified.


class InlineFunctionCallers(object):
    def __init__(self, package_src_root: str, package_bin_dirs: List[str]) -> None:
        self.package_src_root = pathlib.Path(package_src_root).resolve()
        if not self.package_src_root.is_dir():
            print(f"{str(package_src_root)} is not a valid directory")
            return

        self.package_bin_dirs = [pathlib.Path(adir).resolve() for adir in package_bin_dirs]
        for adir in self.package_bin_dirs:
            if not adir.is_dir():
                print(f"{str(adir)} is not a valid directory")
                return

        self.package_binaries = list(itertools.chain([self._find_binaries(adir) for adir in self.package_bin_dirs]))[0]
        self.inline_functions = self._find_inline_functions(self.package_src_root, self.package_binaries)
        self.inline_callers = self._find_inline_function_callers(self.package_src_root, self.inline_functions)

    def is_inline_function_caller(self, function: str) -> bool:
        return function in self.inline_callers

    def _find_binaries(self, path: pathlib.Path) -> List[pathlib.Path]:
        cmd = f"find {str(path)} -type f -executable"
        c = delegator.run(cmd)
        return [pathlib.Path(abin) for abin in c.out.strip().split(os.linesep)]

    def _find_called_functions(self, node: clang.cindex.Cursor) -> Set[str]:
        if node.kind == clang.cindex.CursorKind.CALL_EXPR:
            return set([node.spelling])

        called_functions = set()
        for child in node.get_children():
            called_functions.update(self._find_called_functions(child))

        return called_functions

    def _find_caller_callee_relations(self, root_node: clang.cindex.Cursor, package_tree_dir: str) -> Dict[str, List[str]]:
        caller_callee_relations = {}
        if root_node.kind == clang.cindex.CursorKind.FUNCTION_DECL:
            # We are processing a function. Check if it's in the package or outside(eg: system header)
            if os.path.commonpath([package_tree_dir, root_node.location.file.name]) == os.path.commonpath([package_tree_dir]):
                # Extract function name, path relative to location of source root, start and end line of function
                # TODO: Maybe we should check if the start and end line differ; if not, maybe it's just a declaration and no function
                # body is present?
                fn_name = root_node.spelling
                fn_start_line = root_node.extent.start.line
                fn_end_line = root_node.extent.end.line
                if fn_end_line != fn_start_line:
                    caller_callee_relations[fn_name] = list(self._find_called_functions(root_node))
        else:
            for child in root_node.get_children():
                child_result = self._find_caller_callee_relations(child, package_tree_dir)
                for function_name, function_callees in child_result.items():
                    if function_name in caller_callee_relations:
                        caller_callee_relations[function_name].extend(function_callees)
                        caller_callee_relations[function_name].sort()
                    else:
                        caller_callee_relations[function_name] = function_callees

        return caller_callee_relations

    def _find_inline_functions(self, package_src_root: pathlib.Path, package_binaries: List[pathlib.Path]) -> List[str]:
        """
        Find actually inline functions in a package given it's source tree root and binaries in the package
        """

        inline_functions = set()
        functions_in_binaries = []
        for binary in package_binaries:
            functions_in_binaries.extend(self._get_functions_in_binary(binary))

        source_files: Iterator[str] = find_source_files(package_src_root)
        for source_file in source_files:
            functions_data = find_functions_in_source_file(package_src_root, source_file)
            for functions in functions_data.values():
                for afunction in functions:
                    if afunction["name"] not in functions_in_binaries:
                        inline_functions.add(afunction["name"])

        return list(inline_functions)

    def _find_inline_function_callers(self, package_src_root: pathlib.Path, inline_functions: List[str]) -> Dict[str, str]:
        inline_function_callers = {}
        source_files: Iterator[str] = find_source_files(package_src_root)
        index = clang.cindex.Index.create()
        inline_functions_set = set(inline_functions)
        for source_file in source_files:
            src_file_ast = index.parse(source_file)
            caller_callee_functions = self._find_caller_callee_relations(src_file_ast.cursor, str(package_src_root))
            for caller, callees in caller_callee_functions.items():
                temp_set1 = inline_functions_set.intersection(callees)
                if len(temp_set1) != 0:
                    inline_function_callers[caller] = list(temp_set1)

        return inline_function_callers

    def _get_functions_in_binary(self, binary: pathlib.Path) -> List[str]:
        """
        Extract names of functions in all binaries of a package
        """

        e = ELF(str(binary))
        return e.functions

def create_argument_parser():
    """
    Command line arguments parser
    """

    parser = argparse.ArgumentParser()
    parser.add_argument("-psd", "--package-src-dir", type=str, required=True, help="Package source tree")
    parser.add_argument("-pbd", "--package-bin-dir", type=str, nargs='+', required=True, help="Package binaries directories")
    return parser

def main():
    argparser = create_argument_parser()
    args = argparser.parse_args()

    package_src_root = pathlib.Path(args.package_src_dir).resolve()
    if not package_src_root.is_dir():
        print(f"{str(package_src_root)} is not a valid directory")
        return

    package_bin_dirs = [pathlib.Path(adir).resolve() for adir in args.package_bin_dir]
    for adir in package_bin_dirs:
        if not adir.is_dir():
            print(f"{str(adir)} is not a valid directory")
            return

    inline_functions_callers_finder = InlineFunctionCallers(str(package_src_root), list(map(str, package_bin_dirs)))
    print(inline_functions_callers_finder.inline_callers)

if __name__ == "__main__":
    main()
