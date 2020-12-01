import argparse
import os
import json
import shutil

DECOMP_TAG = "decomp"
RAW_TAG = "raw"

parser = argparse.ArgumentParser(description='convert dataset functions from individual files to dataset inputs')
parser.add_argument('mapping',
                    help='path to mapping.json')
parser.add_argument('generated',
                    help='input path to the generated translations')
parser.add_argument('dataset',
                    help='input path to the dataset used (test/valid/train)')
parser.add_argument('original_raw',
                    help='input path to the original raw code directory')
parser.add_argument('original_decompiled',
                    help='input path to the original decompiled code directory')
parser.add_argument('outpath',
                    help='output path to the output directory')

args = parser.parse_args()

mappings = dict()
with open(args.mapping, "r") as jfile:
    mappings = json.loads(jfile.read())
jfile.close()

def add_newlines(functionRaw):
    # replace ;
    result = functionRaw.replace(";", ";\n")

    # replace {}
    result = result.replace("{", "{\n")
    result = result.replace("}", "\n}\n")
    return result


def replace_unk(line):
    result = line.replace("<unk>", "InN0cmluZyI=")
    return result 

def get_mapped_function(index, mapping):
    if index >= len(mapping):
        return None
        
    return mapping[index]

generated_map = dict()

def splitEntry(line):
    array = line.split("\t", 1)
    prefix = array[0].strip()
    func = array[1].strip()
    return prefix, func

print("Opening ", args.generated)
count = 1
with open(args.generated, "r", encoding='utf8') as genFile:
    for line in genFile:
        if line.startswith("S-"):
            prefix, data = splitEntry(line)
            index = int(prefix.split("-")[1])
            if index in generated_map.keys():
                print("Error - entering key twice")
                exit()
            generated_map[index] = {}
            generated_map[index]["S"] = data # source sample
        elif  line.startswith("T-"):
            prefix, data = splitEntry(line)
            generated_map[index]["T"] = data # target sample
        elif  line.startswith("H-"):
            prefix, data = splitEntry(line)
            array = data.split("\t", 1)
            score = float(array[0].strip())
            funcData = array[1].strip()
            generated_map[index]["H"] = funcData  # hypothesis
            generated_map[index]["H-score"] = score # hypothesis score
        elif  line.startswith("D-"):
            prefix, data = splitEntry(line)
            array = data.split("\t", 1)
            score = float(array[0].strip())
            funcData = array[1].strip()
            generated_map[index]["D"] = funcData # detokenized hypothesis
            generated_map[index]["D-score"] = score # detokenized score
        elif  line.startswith("P-"):
            prefix, data = splitEntry(line)
            generated_map[index]["P"] = data  # source positional scores base 2
        else:
            print("Skipping line %d" % count)
        count+=1
        if count % 1000 == 0:
            print("Parsed %d lines" % count)
print("Done - Parsed %d lines" % count)
genFile.close()

scorePath = os.path.join(args.outpath, "score.csv")

testRawPath = os.path.join(args.dataset, "test.raw")
testsRaw = []
with open(testRawPath, "r", encoding='utf8') as testRawFile:
    for line in testRawFile:
        testsRaw.append(line.strip())
testRawFile.close()



origPath = os.path.join(args.outpath, "abstracted_base64_src_code")
if os.path.exists(origPath):
    shutil.rmtree(origPath)
os.mkdir(origPath)

sinputPath = os.path.join(args.outpath, "sinput")
if os.path.exists(sinputPath):
    shutil.rmtree(sinputPath)
os.mkdir(sinputPath)

resultPath = os.path.join(args.outpath, "result")
if os.path.exists(resultPath):
    shutil.rmtree(resultPath)
os.mkdir(resultPath)

count = 0
total = len(generated_map.keys())
scoreFile = open(scorePath, "w")


mapping = list()
testMapPath = os.path.join(args.dataset, "test.map")
with open(testMapPath, "r", encoding='utf8') as testMapFile:
    for line in testMapFile:
        mapping.append(line.strip())
testMapFile.close()


seen = set()
for index in generated_map.keys():
    dataMap = generated_map[index]
    testRawFunc = testsRaw[index]
    funcName = get_mapped_function(index, mapping)
    # funcName = get_matching_function(testRawFunc, modules, seen)
    if funcName == None:
        print("Unable to find: ")
        print("    --> ", testRawFunc)
    else:
        pass
    result = dataMap["D"]
    dscore = dataMap["D-score"]
    inputline = dataMap["S"]

    newInputFilePath = os.path.join(sinputPath, funcName+".c")
    newOrigFilePath = os.path.join(origPath, funcName+".c")
    newResultFilePath = os.path.join(resultPath, funcName+".c")

    srcPath = os.path.join(args.original_raw, funcName+".c")
    # shutil.copy(srcPath, origPath)
    with open(srcPath, "r", encoding='utf8') as inFile:
        raw = inFile.read()
        raw = raw.replace("\n", " ")
    inFile.close()
    
    # restructedRaw = add_newlines(raw)

    with open(newOrigFilePath, "w", encoding='utf8') as outFile:
        outFile.write(raw)
    outFile.close()

    output = replace_unk(result)
    with open(newResultFilePath, "w") as outFile:
        outFile.write(output)
    outFile.close()

    with open(newInputFilePath, "w") as outFile:
        outFile.write(inputline)
    outFile.close()

    scoreline = "%d,%f,%s.c\n" % (index,dscore, funcName)
    scoreFile.write(scoreline)

    count += 1
    if count % 1000 == 0:
        print("Parsed %d/%d entries" % (count, total))
scoreFile.close()
print("Parsed %d/%d entries" % (count, total))
    # print("Target: ", testRawFunc)