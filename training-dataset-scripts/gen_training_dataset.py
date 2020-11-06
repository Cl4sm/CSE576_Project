import os
import json
import copy
import logging

import tqdm

from placeholder import IDATokenizer, CTokenizer
from utils import new_logger, logger_formatter


##################### CONFIG ###########################
RAW_CODE_DIR = "raw_code"
STRIP_RAW_CODE_DIR = "stripped_raw_code"
DEC_CODE_DIR = "raw_decompiled_code"
STRIP_DEC_CODE_DIR = "stripped_decompiled_code"
MAPPING_NAME = "mapping.json"

##################### INIT ###########################
raw_code_dir = None
strip_code_dir = None
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

def strip_code(code):
    c_tokenizer = CTokenizer()
    tokens = c_tokenizer.tokenize(code)
    return c_tokenizer.detokenize(tokens)

def save_mapping():
    with open(mapping_path, "w") as f:
        json.dump(mapping, f, indent=4)

def save_file(path, content):
    with open(path, 'w') as f:
        f.write(content)

def process_func(func_name, info_dict, pkg_dir, pkg_data):
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
        raw_code = f.read().decode(errors="replace")

    bin_path = last_bin_path
    info = last_info
    dec_func = info[func_name]
    dec_code = dec_func['code']
    strip_raw_code = strip_code(raw_code)
    strip_dec_code = strip_code(dec_code)
    info_bin = copy.deepcopy(info[func_name])
    del info_bin['code']

    # save files
    func_id = id_cnt
    id_cnt += 1
    save_file(os.path.join(raw_code_dir, str(func_id)+'.c'), raw_code)
    save_file(os.path.join(strip_raw_code_dir, str(func_id)+'.c'), strip_raw_code)
    save_file(os.path.join(dec_code_dir, str(func_id)+'.c'), dec_code)
    save_file(os.path.join(strip_dec_code_dir, str(func_id)+'.c'), strip_dec_code)


    # update mapping
    mapping[func_id] = {
                            "func_name": func_name,
                            "package": pkg_name,
                            "binary": bin_path,
                            "info_bin": info_bin,
                            "info_src": pkg_data["functions"][func_name]
                            }
    if id_cnt % 0x1000 == 0:
        save_mapping()

def process_pkg(pkg_dir):
    pkg_dir = os.path.abspath(pkg_dir)
    with open(os.path.join(pkg_dir, "data.json")) as f:
        pkg_data = json.load(f)

    # grab all binary paths first
    bin_paths = []
    for pkg in pkg_data['binaries']:
        bin_paths += [os.path.join(pkg_dir, x['dataset_file']) for x in pkg_data['binaries'][pkg]]

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

    # iterate through functions
    for func_name in pkg_data['functions']:
        logger.debug("processing %s...", func_name)
        try:
            process_func(func_name, info_dict, pkg_dir, pkg_data)
        except Exception as e:
            logger.exception(e)

if __name__ == '__main__':
    import argparse

    # parser
    parser = argparse.ArgumentParser()
    parser.add_argument('-p', '--pkg-dir', help='a package info directory')
    parser.add_argument('-o', '--output-dir', required=True, help='output directory')
    parser.add_argument('-d', '--dataset-dir', help='a dataset directoly full of packages')
    args = parser.parse_args()

    # lol, WTF is this code?
    if not os.path.exists(args.output_dir): os.mkdir(args.output_dir)
    raw_code_dir = os.path.join(args.output_dir, RAW_CODE_DIR)
    if not os.path.exists(raw_code_dir): os.mkdir(raw_code_dir)
    strip_raw_code_dir = os.path.join(args.output_dir, STRIP_RAW_CODE_DIR)
    if not os.path.exists(strip_raw_code_dir): os.mkdir(strip_raw_code_dir)
    dec_code_dir = os.path.join(args.output_dir, DEC_CODE_DIR)
    if not os.path.exists(dec_code_dir): os.mkdir(dec_code_dir)
    strip_dec_code_dir = os.path.join(args.output_dir, STRIP_DEC_CODE_DIR)
    if not os.path.exists(strip_dec_code_dir): os.mkdir(strip_dec_code_dir)
    mapping_path = os.path.join(args.output_dir , MAPPING_NAME)

    if args.pkg_dir:
        assert os.path.isdir(args.pkg_dir), f"{args.pkg_dir} doesn't exist or is not a directory!"
    if args.dataset_dir:
        assert os.path.isdir(args.dataset_dir), f"{args.dataset_dir} doesn't exist or is not a directory!"

    pkgs = os.listdir(args.dataset_dir)
    for pkg in tqdm.tqdm(pkgs, smoothing=0):
        logger.info(f"processing package {pkg}...")
        pkg_dir = os.path.join(args.dataset_dir, pkg)
        process_pkg(pkg_dir)
    save_mapping()
