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


INPUT_DIR = "output/abstracted_src_code"
OUTPUT_DIR = "output/abstracted_base64_src_code"
os.system("mkdir -p %s" % OUTPUT_DIR)
for fname in tqdm.tqdm(os.listdir(INPUT_DIR), smoothing=0, dynamic_ncols=True):
    fpath = os.path.join(INPUT_DIR, fname)
    fid = int(fname.split(".")[0])
    with open(fpath) as f:
        code = f.read()
    c_tokenizer = CTokenizer()
    tokens = c_tokenizer.tokenize(code)
    encoded_tokens = [base64.b64encode(x.encode()).decode() for x in tokens]

    output_fpath = os.path.join(OUTPUT_DIR, fname)
    with open(output_fpath, 'w') as f:
        f.write(" ".join(encoded_tokens))
