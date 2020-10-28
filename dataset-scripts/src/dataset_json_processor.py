import json
import pathlib
import sys
from typing import Dict, List, Tuple, Union

import allstar


class DatasetJSONProcessor(object):
    def __init__(self, json_files: List[str], sources_dirs: List[pathlib.Path], out_dir: pathlib.Path, log_fh=sys.stdout):
        self.log_fh = log_fh
        print(f"Intializing Allstar repo agent...", end='', flush=True, file=self.log_fh)
        self.allstar_repo = allstar.AllstarRepo("amd64")
        print("done", file=self.log_fh)
        self.json_files = [pathlib.Path(afile).resolve() for afile in json_files]
        self.out_dir = pathlib.Path(out_dir)
        self.sources_dirs = [pathlib.Path(adir) for adir in sources_dirs]

        self.out_dir.mkdir(exist_ok=True)

    def create_dataset(self) -> None:
        for json_file in self.json_files:
            self._process_json_file(json_file)

    def _extract_function(self, src_file: pathlib.Path, start: int, end: int, out_file: pathlib.Path) -> None:
        """
        Extract code of one function from src_file into out_file
        """

        ifh = src_file.open('rb')
        for _ in range(1, start):
            ifh.readline()

        ofh = out_file.open('wb')
        for _ in range(start, end + 1):
            ofh.write(ifh.readline())

        ifh.close()
        ofh.close()

    def _extract_functions(self, src_dir: pathlib.Path, functions: Dict, package_data_out_dir: pathlib.Path):
        """
        Extract code of all functions. Returns Dict[str, Dict[str, Union[str, int]]].
        """

        functions_details = {}
        src_out_dir = package_data_out_dir / "src"
        src_out_dir.mkdir(exist_ok=True)
        for src_file, src_functions in functions.items():
            for afunction in src_functions:
                if "/" in afunction["name"]:
                    afunction_out_file = src_out_dir / f"{afunction['name'].replace('/', '-forwardslash-')}.c"
                else:
                    afunction_out_file = src_out_dir / f"{afunction['name']}.c"
                self._extract_function(src_dir / src_file, afunction["start"], afunction["end"], afunction_out_file)
                function_record = {"orig_source_file": src_file, "orig_start_line": afunction["start"],
                                   "orig_end_line": afunction["end"],
                                   "dataset_file": str(afunction_out_file.relative_to(package_data_out_dir))}
                functions_details[afunction["name"]] = function_record

        return functions_details

    def _find_package_source_location(self, package: str) -> Union[pathlib.Path, None]:
        """
        Find packge's sources directory
        """

        for adir in self.sources_dirs:
            if (adir / package / "debian").is_dir():
                return (adir / package)

    def _get_package_allstar_config(self, package: str, package_out_dir: pathlib.Path) -> Dict:
        """
        Retrieve and cache allstar config for package. Attempt to convert to JSON.
        """

        package_allstar_data_folder = package_out_dir / "allstar-data"
        package_allstar_data_folder.mkdir(exist_ok=True)
        package_index_out_file = package_allstar_data_folder / f"{package}-allstar.json"
        if package_index_out_file.is_file():
            with package_index_out_file.open() as fh:
                try:
                    return json.load(fh)
                except json.decoder.JSONDecodeError:
                    # Let's try fetching the data again. Maybe it's corrupted
                    pass

        try:
            package_index_str = self.allstar_repo._package_index_raw(package)
        except KeyError:
            # No data available for package. Log and ignore
            with package_index_out_file.open('w') as fh:
                fh.write("{}")
            print(f"No JSON data found for {package} in Allstar repo", file=self.log_fh)
            return {}

        with package_index_out_file.open('w') as fh:
            fh.write(package_index_str)

        try:
            return json.loads(package_index_str)
        except json.decoder.JSONDecodeError:
            print(f"Unable to JSON decode package info. Contents dumped to {package_index_out_file}", file=self.log_fh)

    def _get_package_binaries_details(self, package: str, package_allstar_info: Dict, package_data_out_dir: pathlib.Path):
        """
        Get names of executables and their URLs on Allstar repos. Returns List[Dict[str, str]].
        """

        binaries = self.allstar_repo.package_binaries_details(package, package_allstar_info)
        binaries_details = []
        binary_out_dir = package_data_out_dir / "binaries"
        for binary in binaries:
            if binary["name"].endswith(".so"):
                continue

            binary_out_file = (binary_out_dir / package / binary["name"]).relative_to(package_data_out_dir)
            binary_rec = {"name": binary["name"], "url": binary["url"], "dataset_file": str(binary_out_file)}
            binaries_details.append(binary_rec)

        return binaries_details


    def _get_package_names_in_debian_repos(self, package_src_dir: pathlib.Path) -> Tuple[str, List[str]]:
        """
        Get source and deb package names for package
        """

        control_file = package_src_dir / "debian" / "control"
        deb_package_names = []
        source_package_name = ""
        with control_file.open() as fh:
            for aline in fh:
                if aline.startswith("Source: "):
                    source_package_name = aline.strip().split(": ")[1]
                elif aline.startswith("Package: "):
                    deb_package_names.append(aline.strip().split(": ")[1])

        return (source_package_name, deb_package_names)

    def _process_json_file(self, json_file: pathlib.Path) -> None:
        with open(json_file) as fh:
            json_data = json.load(fh)

        print(f"Processing {json_file.stem}...", end="", flush=True, file=self.log_fh)
        for package, package_functions_data in json_data.items():
            package_source_dir = self._find_package_source_location(package)
            if package_source_dir is None:
                print(f"Unable to find source directory for {package}", file=self.log_fh)
                continue

            # Get package names
            src_package, deb_packages = self._get_package_names_in_debian_repos(package_source_dir)
            package_data_out_dir = self.out_dir / src_package
            package_data_out_file = package_data_out_dir / "data.json"
            if package_data_out_file.is_file():
                # This package is already processed. No need to process again.
                continue

            errored = False
            package_data_out_dir.mkdir(exist_ok=True)
            # Find all executables in the package from Allstar repo
            package_binaries_details = {}
            for deb_package in deb_packages:
                package_allstar_info = self._get_package_allstar_config(deb_package, package_data_out_dir)
                if package_allstar_info == {}:
                    continue
                elif package_allstar_info is None:
                    errored = True
                    continue
                package_binaries_details[deb_package] = self._get_package_binaries_details(deb_package, package_allstar_info, package_data_out_dir)
            if errored:
                # Some package's Allstar metadata is corrupted. Defer processing source package until it's fixed.
                continue
            # Extract functions in package
            package_functions = self._extract_functions(package_source_dir, package_functions_data, package_data_out_dir)
            # Dump package info
            package_data = {"source_package": src_package, "orig_package_src_dir": package, "deb_packages": deb_packages,
                            "binaries": package_binaries_details, "functions": package_functions}

            with package_data_out_file.open("w") as fh:
                json.dump(package_data, fh, indent=4)

        print("done.", file=self.log_fh)

