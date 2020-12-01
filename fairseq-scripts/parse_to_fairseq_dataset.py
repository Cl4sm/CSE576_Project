
import argparse
import os
import shutil
from sklearn.model_selection import train_test_split
import json

TEST_SIZE = 0.15
VALID_SIZE = 0.10
TRAIN_SIZE = 0.75

DECOMP_TAG = "decomp"
RAW_TAG = "raw"

parser = argparse.ArgumentParser(description='convert dataset functions from individual files to dataset inputs')
parser.add_argument('mapping',
                    help='path to mapping.json')
parser.add_argument('raw',
                    help='path to the raw code directory')
parser.add_argument('decompiled',
                    help='path to the decompiled code directory')
parser.add_argument('outfile',
                    help='name of output folder')

args = parser.parse_args()

rawbasepath = args.raw
decompiledbasepath = args.decompiled
outpath = args.outfile
rawfuncList = os.listdir(rawbasepath)
decompiledfuncList = os.listdir(decompiledbasepath)

mappingbasepath = args.mapping

print(mappingbasepath)
print(rawbasepath)
print(decompiledbasepath)
print("getting %d raw funcs" % len(rawfuncList))
print("getting %d decompiled funcs" % len(decompiledfuncList))

mappings = dict()
with open(mappingbasepath, "r") as jfile:
    mappings = json.loads(jfile.read())
jfile.close()

check = []
rawfuncList.sort()
for name in rawfuncList:
    numline = name.split('.')[0]
    num = int(numline)
    check.append(num)

check.sort()
prev = -1
for i in check:
    if i != prev + 1:
        print("file skipped!", prev, "->", i)
    prev = i

decompiledfuncList.sort()

check = []
for name in decompiledfuncList:
    numline = name.split('.')[0]
    num = int(numline)
    check.append(num)

check.sort()
prev = -1
skipped = set()
for i in check:
    if i != prev + 1:
        print("file skipped!", prev, "->", i)
        for idx in range(prev + 1, i):
            skipped.add(idx)
    prev = i

print(skipped)

if os.path.exists(outpath):
    shutil.rmtree(outpath)
os.mkdir(outpath)

if len(rawfuncList) != len(decompiledfuncList):
    print("Error none matching number of funcs in folders")
    exit()

count = 0
modules = dict()

functions = mappings.keys()
total = len(functions)

for funcName in functions:
    if int(funcName) in skipped:
        print("Skipping " + funcName)
        continue

    rawfuncPath = os.path.join(rawbasepath, funcName+".c")
    decompfuncPath = os.path.join(decompiledbasepath, funcName+".c")

    rawentry = ""
    with open(rawfuncPath, 'r', encoding='utf8') as funcFile:
        for line in funcFile:
            line = line.strip()
            rawentry += line+" "
    funcFile.close()
    rawentry.strip()
    rawentry = rawentry+'\n'

    decompentry = ""
    with open(decompfuncPath, 'r', encoding='utf8') as funcFile:
        for line in funcFile:
            line = line.strip()
            decompentry += line+" "
    funcFile.close()
    decompentry.strip()
    decompentry = decompentry+'\n'

    rawEntryLen = len(rawentry.split()) 
    decompEntryLen = len(decompentry.split()) 
    # if rawEntryLen > 4096 or decompEntryLen > 4096:
    #   print("Skipping func[%s] size = %d | %d " % (funcName, rawEntryLen, decompEntryLen))
    #   continue

    package = mappings[funcName]['package']
    if package not in modules.keys():
        modules[package] = []
    modules[package].append((rawentry, decompentry, funcName))

    count += 1
    if count % 1000 == 0:
        print("Got %d/%d files" % (count, total))


print("Got %d files" % count)

all_packages = list(modules.keys())
bigtrain, testset = train_test_split(all_packages, test_size=TEST_SIZE)
relative_validsize = VALID_SIZE/(TRAIN_SIZE + VALID_SIZE)
trainset, validset = train_test_split(bigtrain, test_size=relative_validsize)

print("Splitting %d modules into:" % len(all_packages))
print("    - %d train" % len(trainset))
print("    - %d test" %  len(testset))
print("    - %d valid" % len(validset))

outputMap = {'train': trainset, 'test': testset, 'valid': validset}
outputTags = [RAW_TAG, DECOMP_TAG]


for outType in outputMap.keys():
    rawPath = os.path.join(outpath, outType+"."+RAW_TAG)
    decompPath = os.path.join(outpath, outType+"."+DECOMP_TAG)
    mapPath = os.path.join(outpath, outType+"."+"map")
    packages = outputMap[outType]
    with open(rawPath, 'a', encoding='utf8') as rawFile:
        with open(decompPath, 'a', encoding='utf8') as decompFile:
            for package in packages:
                data = modules[package]

                for entry in data:
                    rawData = entry[0]
                    decompData = entry[1]
                    funcName = entry[2]
                    rawFile.write(rawData)
                    decompFile.write(decompData)
                    with open(mapPath, 'a', encoding='utf8') as mapFile:
                        mapFile.write(funcName+'\n')
                    mapFile.close()
    rawFile.close()
    decompFile.close()
print("Done!")
