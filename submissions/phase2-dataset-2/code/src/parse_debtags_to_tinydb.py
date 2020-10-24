#!/usr/bin/env python3

import os
import sys

import tinydb

# Parse debtags file and insert into tinydb database


def create_argument_parser():
    parser = argparse.ArgumentParser()
    parser.add_argument("-df", "--debtags-file", type=str, required=True, help="Debtags file")
    parser.add_argument("-j", "--json", type=str, required=False, default="debtags.json",
                        help="Output JSON file. Will be clobbered.")
    return parser


def main(debtags_file, debtags_json_out):
    if not os.path.isfile(debtags_file):
        print(f"{debtags_file} is not a valid file", file=sys.stderr)
        return

    packages_tags_data = []
    fh = open(debtags_file)
    while True:
        aline = fh.readline()
        if not aline:
            break

        package_name, tag_list = aline.strip().split(": ")
        package_tag_data = {}
        for tag in tag_list.split(", "):
            tag_key, tag_value = map(lambda x: x.lower(), tag.split("::"))
            if tag_key in package_tag_data:
                package_tag_data[tag_key].append(tag_value)
            else:
                package_tag_data[tag_key] = [tag_value]

        for tag_key, tag_value in package_tag_data.items():
            package_tag_data[tag_key] = ";".join(sorted(tag_value))

        package_tag_data["package_name"] = package_name
        packages_tags_data.append(package_tag_data)

    fh.close()
    db = tinydb.TinyDB(debtags_json_out, indent=4)
    db.insert_multiple(packages_tags_data)
    db.close()
    return

if __name__ == "__main__":
    import argparse
    args_parser = create_argument_parser()
    args = args_parser.parse_args()

    main(args.debtags_file, args.json)
