#!/usr/bin/env python3

import os

import tinydb

# Level 1: Classify packages using debtags into following categories
# - not runnable
# - unknown if runnable
# - runnable but not C
# - runnable but language unknown
# - runnable and partially in C
# - runnable and fully in C

# Some debtags notes
# implemented-in: Language a package is implemented in. Can be multiple: stored semi-colon separated in DB.
# role: A role assigned to a package. "program" role alone is assumed to contain a runnable program


def create_argument_parser():
    parser = argparse.ArgumentParser()
    parser.add_argument("--db", type=str, required=True, help="Debtags TinyDB file")
    parser.add_argument("-od", "--output-dir", type=str, help="Directory for outputs.", default="output")
    return parser


def main(db_file, output_dir):

    if not os.path.isfile(db_file):
        print(f"{db_file} is not a valid file")
        return

    db = tinydb.TinyDB(db_file)
    q = tinydb.Query()
    if not os.path.isabs(output_dir):
        output_dir = os.path.join(os.getcwd(), output_dir)

    os.makedirs(output_dir, exist_ok=True)

    check_if_runnable_package_file = os.path.join(output_dir, "check-if-runnable-package.txt")
    partially_c_packages_file = os.path.join(output_dir, "partially-c-packages.txt")
    not_runnable_packages_file = os.path.join(output_dir, "not-runnable-packages.txt")
    runnable_non_c_packages_file = os.path.join(output_dir, "non-c-runnable-packages.txt")
    runnable_unknown_language_packages_file = os.path.join(output_dir, "runnable-unknown-language-packages.txt")
    runnable_c_only_packages_file = os.path.join(output_dir, "runnable-c-only-packages.txt")

    # Find all packages that do not contain a runnable program
    not_runnable_packages = set(package["package_name"] for package in db.search(q.role != "program"))

    # Find all packages for which we do not know if a runnable program is present or not
    check_if_runnable_package = set(package["package_name"] for package in db.search(~q.role.exists()))

    # Find all packages that contain a runnable program and contain only C code
    runnable_c_only_packages = set(package["package_name"] for package in db.search((q.role == "program") & (q["implemented-in"] == "c")))

    # Find all packages that contain a runnable program and some C code
    partially_c_packages = set(package["package_name"] for package in db.search(((q.role == "program") &
                                                                                 (q["implemented-in"].matches("c;")))))

    # Find all packages that contain a runnable program but do not contain any C code
    # We need to ignore matches with "c;" and not matches with "c" because "c++" also matches "c". That's why the additional
    # check != "c"
    runnable_non_c_packages = set(package["package_name"] for package in db.search((q.role == "program") &
                                                                                   (q["implemented-in"].exists()) &
                                                                                   (~q["implemented-in"].matches("c;")) &
                                                                                   (q["implemented-in"] != "c")))

    # Find all packages that contain a runnable program whose language is not known
    runnable_unknown_language_packages = set(package["package_name"] for package in db.search((q.role == "program") &
                                                                                              (~q["implemented-in"].exists())))

    # Sanity check to ensure no overlap: classification should be into disjoint sets
    all_sets = {"not_runnable_packages": not_runnable_packages,
                "check_if_runnable_package": check_if_runnable_package,
                "runnable_c_only_packages": runnable_c_only_packages,
                "partially_c_packages": partially_c_packages,
                "runnable_non_c_packages": runnable_non_c_packages,
                "runnable_unknown_language_packages": runnable_unknown_language_packages
                }

    all_sets_keys = list(all_sets.keys())

    failed = False
    for i in range(len(all_sets_keys)):
        for j in range(i + 1, len(all_sets_keys)):
            if len(all_sets[all_sets_keys[i]].intersection(all_sets[all_sets_keys[j]])) != 0:
                print(f"{all_sets_keys[i]} and {all_sets_keys[j]} overlap")
                failed = True

    if failed:
        print("Not writing package names since some sets overlap")
        return

    print("Writing package names...", end="")
    # Dump package names
    fh = open(check_if_runnable_package_file, 'w')
    for package in sorted(check_if_runnable_package):
        fh.write(package + os.linesep)

    fh.close()

    fh = open(partially_c_packages_file, 'w')
    for package in sorted(partially_c_packages):
        fh.write(package + os.linesep)

    fh.close()

    fh = open(not_runnable_packages_file, 'w')
    for package in sorted(not_runnable_packages):
        fh.write(package + os.linesep)

    fh.close()

    fh = open(runnable_non_c_packages_file, 'w')
    for package in sorted(runnable_non_c_packages):
        fh.write(package + os.linesep)

    fh.close()

    fh = open(runnable_unknown_language_packages_file, 'w')
    for package in sorted(runnable_unknown_language_packages):
        fh.write(package + os.linesep)

    fh.close()

    fh = open(runnable_c_only_packages_file, 'w')
    for package in sorted(runnable_c_only_packages):
        fh.write(package + os.linesep)

    fh.close()
    print("done")


if __name__ == "__main__":
    import argparse
    arg_parser = create_argument_parser()
    args = arg_parser.parse_args()

    main(args.db, args.output_dir)
