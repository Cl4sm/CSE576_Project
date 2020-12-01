import os
import json
import copy
import glob
import base64
import random
import string
import logging
import subprocess

import tqdm
import clang
from clang.cindex import TokenKind, CursorKind

from placeholder import IDATokenizer, CTokenizer
from utils import new_logger, logger_formatter, path_context


def abstract_code(code, build_dir=None, build_src_path=None):
    try:
        c_tokenizer = CTokenizer(build_dir=build_dir, build_src_path=build_src_path)
        tokens = c_tokenizer.tokenize(code)
    except Exception:
        c_tokenizer = CTokenizer()
        tokens = c_tokenizer.tokenize(code)
    return c_tokenizer.detokenize(tokens)


#INPUT_DIR = "/dev/shm/output/abstracted_decompiled_code"
#OUTPUT_DIR = "/dev/shm/output/abstracted_base64_decompiled_code"


INPUT_DIR_BASE = "./full_base64"
OUTPUT_DIR_BASE = "./full_base64_decoded"
for sub_dir in ["sinput", "abstracted_base64_src_code",  "result"]:
    input_dir = os.path.join(INPUT_DIR_BASE, sub_dir)
    output_dir = os.path.join(OUTPUT_DIR_BASE, sub_dir)
    os.system("mkdir -p %s" % input_dir)
    os.system("mkdir -p %s" % output_dir)
    for fname in tqdm.tqdm(os.listdir(input_dir), smoothing=0, dynamic_ncols=True):
        fpath = os.path.join(input_dir, fname)
        fid = int(fname.split(".")[0])
        with open(fpath) as f:
            code = f.read()
    
        tokens = []
        for x in code.split():
            try:
                y = base64.b64decode(x).decode('utf8', 'ignore')
            except Exception:
                y = '<unk>'
            tokens.append(y)
        c_tokenizer = CTokenizer()
        output_code = c_tokenizer.detokenize(tokens)
    
        output_fpath = os.path.join(output_dir, fname)
        with open(output_fpath, 'w') as f:
            f.write(output_code)
