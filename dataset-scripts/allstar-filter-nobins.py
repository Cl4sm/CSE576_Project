#!/usr/bin/env python3

# filter out allstar packages with no corresponding
# binaries on their repo

import allstar
import argparse
import os
import shutil

parser = argparse.ArgumentParser(description='filter out packages that dont have binaries from Allstar repo')
parser.add_argument('directory',
                    help='path to the directory where the packages are')

args = parser.parse_args()
repo = allstar.AllstarRepo("amd64")
all_packages = repo.package_list()
package_files = os.listdir(args.directory)
map_filepath = os.path.dirname(args.directory)+"-names.txt"

package_map = dict()


with open(map_filepath, "r") as mapFile:
    for line in mapFile:
        lineArray = line.split(' ')
        fileArray = lineArray[0].split('/')
        fileName = fileArray[0].strip()
        packageName = lineArray[1].strip()
        if fileName not in package_map.keys():
                package_map[fileName] = []
        package_map[fileName].append(packageName)

print("Map file generated from %s" % map_filepath)

done = set()
has_bins = list()
errors = dict()
missing = list()
nomapping = list()

all_packages.sort()
package_files.sort()

count = 0
for pfile in package_files:
    pname = pfile.split(".json")[0]
    if pname not in package_map.keys():
        print("No corresponding package mapping found for %s" % pname)
        if pfile not in nomapping:
            nomapping.append(pfile)
        continue

    packages = package_map[pname]

    found = False
    errored = False
    pot_errors = []
    # found match
    print("Grabbing packages for %s" % (pname))
    for package_name in packages:
        print("   - %s" % package_name)
        try:
            index = repo._package_index(package_name)
        except Exception as e1:
            print("   ---> ERROR %s" % e1)

            errorMessage = "%s: %s" % (package_name, e1)
            pot_errors.append(errorMessage)
            found = True
            errored = True
            continue

        bins = index['binaries']
        if len(bins) > 0:
            has_bins.append(pfile)
            count += 1
            found = True
            errored = False
            break

    if errored:
        if pfile not in errors.keys():
            errors[pfile] = []
        errors[pfile].extend(pot_errors)

    elif not found:
        if pfile not in missing:
            missing.append(pfile)


print("Found %d/%d Packages with binaries on repo:" % (count, len(package_files)))

filtered_dir = args.directory.strip('/')+"-hasbinaries"
orignal_dir = args.directory.strip('/')
if os.path.exists(filtered_dir):
    shutil.rmtree(filtered_dir)
os.mkdir(filtered_dir)

for bins in has_bins:
    print("   - ", bins)
    src = orignal_dir+'/'+bins
    dst = filtered_dir+'/'+bins
    shutil.copyfile(src, dst)


print("\n %d/%d Files without matching packages:" % (len(nomapping), len(package_files)))
for bins in nomapping:
    print("   - ", bins)

print("\n %d/%d Files with missing packages in the repo:" % (len(missing), len(package_files)))
for bins in missing:
    print("   - ", bins)

print("\n %d/%d Packages that errored during retrieval:" % (len(errors), len(package_files)))
for bins in errors.keys():
    print("   - ", bins)
    for errormessage in errors[bins]:
        print("       > ", errormessage)