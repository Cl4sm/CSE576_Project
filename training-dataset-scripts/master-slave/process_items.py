"""
user specified functions
"""
import os
import json
import glob
import copy
import base64
import logging
import subprocess
import multiprocessing

import tqdm
import clang
from clang.cindex import TokenKind, CursorKind

from placeholder import IDATokenizer, CTokenizer
from utils import new_logger, logger_formatter, path_context

#####--------Configuration----------######
SRC_CODE_DIR = "src_code"
ABSTRACTED_SRC_CODE_DIR = "abstracted_src_code"
DEC_CODE_DIR = "decompiled_code"
ABSTRACTED_DEC_CODE_DIR = "abstracted_decompiled_code"
MAPPING_NAME = "mapping.json"
FUNC_NAME_BLACKLIST = ["start"]
COMPILE_DB_DIR = "/home/kylebot/Desktop/courses/CSE576/datasets/compiled_bin_dataset/compile-db"
output_dir = "./output"
dataset_dir = "/home/kylebot/Desktop/courses/CSE576/datasets/dataset_v2"

clang.cindex.Config.set_library_path('/usr/local/lib/')

#####--------Initialization----------######
src_code_dir = None
abstracted_code_dir = None
dec_code_dir = None
mapping_path = None

id_cnt = 0 
mapping = {}

logger = new_logger("Dataset")
logger.setLevel("INFO")
handler = logging.FileHandler("log.txt", mode='a', delay=False)
handler.setLevel("DEBUG")
handler.setFormatter(logger_formatter)
logger.addHandler(handler)

if not os.path.exists(output_dir): os.mkdir(output_dir)
src_code_dir = os.path.join(output_dir, SRC_CODE_DIR)
if not os.path.exists(src_code_dir): os.mkdir(src_code_dir)
abstracted_src_code_dir = os.path.join(output_dir, ABSTRACTED_SRC_CODE_DIR)
if not os.path.exists(abstracted_src_code_dir): os.mkdir(abstracted_src_code_dir)
dec_code_dir = os.path.join(output_dir, DEC_CODE_DIR)
if not os.path.exists(dec_code_dir): os.mkdir(dec_code_dir)
abstracted_dec_code_dir = os.path.join(output_dir, ABSTRACTED_DEC_CODE_DIR)
if not os.path.exists(abstracted_dec_code_dir): os.mkdir(abstracted_dec_code_dir)
mapping_path = os.path.join(output_dir , MAPPING_NAME)

def abstract_code(code, build_dir=None, build_src_path=None):
    try:
        c_tokenizer = CTokenizer(build_dir=build_dir, build_src_path=build_src_path)
        tokens = c_tokenizer.tokenize(code)
    except Exception:
        c_tokenizer = CTokenizer()
        tokens = c_tokenizer.tokenize(code)
    return c_tokenizer.detokenize(tokens)

def save_mapping():
    with open(mapping_path, "w") as f:
        json.dump(mapping, f, indent=4)

def save_file(path, content):
    with open(path, 'w') as f:
        f.write(content)

#####--------MASTER FUNCTIONS-------######
def get_item_from_func(func_name, info_dict, pkg_dir, pkg_data, compile_info):
    global id_cnt
    pkg_name = pkg_data['source_package']

    cnt = 0
    last_bin_path = None
    last_info = None
    for bin_path, info in info_dict.items():
        if func_name in info:
            cnt += 1
            last_bin_path = bin_path
            last_info = info
    if cnt == 0:
        logger.debug(f"[-] Can't find function {func_name} in package {pkg_name}")
        return
    if cnt > 1:
        logger.warning(f"[-] multiple definitions for {func_name} in package {pkg_name}")
        return
    logger.info(f"[+] found exactly one definition of {func_name} in package {pkg_name}")
    with open(os.path.join(pkg_dir, 'src', func_name+'.c'), "rb") as f:
        src_code = f.read().decode(errors="replace")
    if '#ifdef' in src_code:
        logger.warning(f"function {func_name} from package {pkg_name} has ifdef!")
        return

    bin_path = last_bin_path
    info = last_info
    info_func = pkg_data["functions"][func_name]
    dec_func = info[func_name]
    dec_code = dec_func['code']
    src_path = info_func["orig_source_file"]
    compile_key = os.path.join("/inline-macro-elimination-dataset/packages-archives/", pkg_name, pkg_data["orig_package_src_dir"], src_path)

    # look for the compilation entry
    for entry in compile_info["compile-db"]:
        if entry['file'] == compile_key:
            break
    else:
        raise

    # look for source code directory
    pkg_dirname = os.path.basename(compile_info["src_root"])
    pkg_dirpaths = glob.glob(os.path.join("/home/kylebot/Desktop/courses/CSE576/src/CSE576_Project/dataset-scripts/output/dataset-packages/*/", pkg_dirname))
    assert len(pkg_dirpaths) >= 1, pkg_dirpaths
    pkg_dirpath = pkg_dirpaths[0]

    # translate build directory
    fake_build_dir = entry['directory']
    build_dirname = fake_build_dir.split(pkg_dirname)[1][1:]
    build_dir = os.path.join(pkg_dirpath, build_dirname)
    build_src_path = os.path.relpath(src_path, start=build_dirname)
    assert os.path.exists(build_dir)
    assert os.path.exists(os.path.join(build_dir, build_src_path))

    info_bin = copy.deepcopy(info[func_name])
    del info_bin['code']

    func_id = id_cnt
    id_cnt += 1
    map_entry = {
                    "func_name": func_name,
                    "package": pkg_name,
                    "binary": bin_path,
                    "info_bin": info_bin,
                    "info_src": info_func
                    }
    return {'func_id': func_id, 'src_code': src_code, 'build_dir': build_dir, 'build_src_path': build_src_path, 'dec_code': dec_code, 'map_entry': map_entry}

    #abstracted_src_code = abstract_code(src_code, build_dir=build_dir, build_src_path=build_src_path)
    #abstracted_dec_code = abstract_code(dec_code)
    #info_bin = copy.deepcopy(info[func_name])
    #del info_bin['code']

def get_items_from_pkg(pkg_dir):
    pkg_dir = os.path.abspath(pkg_dir)
    with open(os.path.join(pkg_dir, "data.json")) as f:
        pkg_data = json.load(f)
    pkg_name = pkg_data["source_package"]
    with open(os.path.join(COMPILE_DB_DIR, pkg_name+".json")) as f:
        compile_info = json.load(f)

    # grab all binary paths and filtering them by existence first
    bin_paths = []
    for pkg in pkg_data['binaries']:
        bin_paths += [os.path.join(pkg_dir, x['dataset_file']) for x in pkg_data['binaries'][pkg]]
    bin_paths = [x for x in bin_paths if os.path.exists(x)]

    # now use IDATokenizer to grab info from the binaries
    ida_tokenizer = IDATokenizer()
    info_dict = {}
    for path in bin_paths:
        try:
            info = ida_tokenizer.get_bin_info(path)
            info = info['data']
            info_dict[os.path.basename(path)] = info
        except Exception as e:
            logger.exception(e)

    item_list = []
    # iterate through functions
    for func_name in pkg_data['functions']:
        if func_name in FUNC_NAME_BLACKLIST:
            continue
        logger.debug("processing %s...", func_name)
        try:
            item = get_item_from_func(func_name, info_dict, pkg_dir, pkg_data, compile_info)
            if item:
                item_list.append(item)
        except Exception as e:
            logger.exception(e)
    return item_list

def get_all_items():
    """
    return a list of items
    """
    # use this code for better debugging
    #with open('./state.json') as f:
    #    state = json.load(f)
    #links = [x[1] for x in state['targets']]
    item_list = []
    pkgs = os.listdir(dataset_dir)
    for pkg in tqdm.tqdm(pkgs, smoothing=0, dynamic_ncols=True):
        pkg_dir = os.path.join(dataset_dir, pkg)
        item_list += get_items_from_pkg(pkg_dir)
    print(len(item_list))
    return item_list

def process_task_result(task_res):
    """
    what to do with the task result
    the argument is the return value of `process_task` of slave functions
    """
    pass

def populate(task):
    """
    what info do you want to send about a task
    the argument is the return value of `process_task` of slave functions
    """
    return task

#def finish_hook():
#    pass

#####--------SLAVE FUNCTIONS-------######
def process_task(task):
    """
    process a task and append result to the task, a task is a dictionary
    the initial item can be accessed by `task['item']`
    """
    tid = task['task_id']
    task_res = {
        "task_id": tid,
        "result": None
        }
    item = task['item']


    func_id = item['func_id']
    src_code = item['src_code']
    dec_code = item['dec_code']
    build_dir = item['build_dir']
    build_src_path = item['build_src_path']
    map_entry = item['map_entry']


    abstracted_src_code = abstract_code(src_code, build_dir=build_dir, build_src_path=build_src_path)
    abstracted_dec_code = abstract_code(dec_code)
    save_file(os.path.join(src_code_dir, str(func_id)+'.c'), src_code)
    save_file(os.path.join(abstracted_src_code_dir, str(func_id)+'.c'), abstracted_src_code)
    save_file(os.path.join(dec_code_dir, str(func_id)+'.c'), dec_code)
    save_file(os.path.join(abstracted_dec_code_dir, str(func_id)+'.c'), abstracted_dec_code)


    # update mapping
    mapping[func_id] = map_entry

    if func_id % 0x1000 == 0:
        save_mapping()

    return func_id

with open("items.json") as f:
    items = json.load(f)

pool = multiprocessing.Pool(5)

tasks = [{'task_id': i, 'item': items[i]} for i in range(len(items))]
it = pool.imap_unordered(process_task, tasks)

with tqdm.tqdm(total=len(tasks), smoothing=0, dynamic_ncols=True) as pbar:# show progress
    while True:
        try:
            tid = it.next()
            pbar.update(1)
        except StopIteration:
            save_mapping()
            break
        except Exception as e:
            logger.exception(e)
            save_mapping()

#for task in tqdm.tqdm(tasks, smoothing=0, dynamic_ncols=True):
#    process_task(task)
save_mapping()

