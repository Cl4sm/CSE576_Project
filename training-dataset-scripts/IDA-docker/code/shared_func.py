"""
user specified functions
"""
import os
import json
import base64
import subprocess

import jsonlines

from placeholder import IDATokenizer
from utils import new_logger, tmp_path

logger = new_logger("Zipper")

#####--------Configuration----------######
DATASET_PATH = "/home/kylebot/Desktop/courses/CSE576/datasets/dataset/dataset"

#####--------MASTER FUNCTIONS-------######
def get_all_items():
    """
    return a list of items
    """
    # use this code for better debugging
    #with open('./state.json') as f:
    #    state = json.load(f)
    #links = [x[1] for x in state['targets']]
    pkg_paths = [os.path.join(DATASET_PATH, x) for x in os.listdir(DATASET_PATH)]
    return pkg_paths

def process_task_result(task_res):
    """
    what to do with the task result
    the argument is the return value of `process_task` of slave functions
    """
    # save the decompilation cache
    assert 'result' in task_res
    if not task_res['result']:
        return

    cache_dir = "/tmp/ida_tokenizer_cache/"
    os.system(f"mkdir {cache_dir}")
    with tmp_path(suffix=".zip") as zip_path, tmp_path(suffix="_dir") as dir_path:
        with open(zip_path, 'wb') as f:
            f.write(base64.b64decode(task_res['result']))
        os.system(f"unzip -d {dir_path} {zip_path} > /dev/null 2>&1")
        os.system(f"mv {dir_path}/*/* {cache_dir}")

def populate(task):
    """
    what info do you want to send about a task
    the argument is the return value of `process_task` of slave functions
    """
    pkg_path = task['item']
    pkg_name = os.path.basename(pkg_path)
    dataset_dir = os.path.dirname(pkg_path)
    with tmp_path(suffix='.zip') as fpath:
        logger.debug(f"zipping package {pkg_name} into {fpath}")
        os.system(f"cd {dataset_dir} && zip -r '{fpath}' '{pkg_name}' > /dev/null 2>&1")
        with open(fpath, 'rb') as f:
            content = f.read()
    item = {}
    item['pkg_name'] = pkg_name
    item['zip'] = base64.b64encode(content).decode()
    logger.debug(f"zipping package {pkg_name} completed")

    task['item'] = item
    return task

#def finish_hook():
#    pass

#####--------SLAVE FUNCTIONS-------######
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

def process_task(task):
    """
    process a task and append result to the task, a task is a dictionary
    the initial item can be accessed by `task['item']`
    """
    assert 'item' in task and 'task_id' in task, "malformed task: %s" % json.dumps(task, indent=4)
    item = task['item']
    tid = task['task_id']
    task_res = {
        "task_id": tid,
        "result": None
        }
    pkg_name = item['pkg_name']
    zip_content = item['zip']

    logger.info(f"processing package {pkg_name}...")

    # lol, create an empty folder just in case
    os.system("/tmp/ida_tokenizer_cache")
    with tmp_path(suffix='.zip') as zip_path, tmp_path(suffix='_dir') as tmp_dir:
        # write zip file
        with open(zip_path, "wb") as f:
            content = base64.b64decode(zip_content)
            f.write(content)

        # unzip the zip file into a temporary directory
        os.system(f"unzip -d {tmp_dir} {zip_path}")

        # now build decompilation cache using IDA
        pkg_dir = os.path.join(tmp_dir, pkg_name)
        process_pkg(pkg_dir)

        # now zip the cache folder and send all of them back
        os.system("cd /tmp; mkdir -p ida_tokenizer_cache; zip -r ida_tokenizer_cache.zip ida_tokenizer_cache >/dev/null 2>&1")
        with open("/tmp/ida_tokenizer_cache.zip", 'rb') as f:
            content = f.read()

    # lol, clean up
    os.system('rm -rf /tmp/ida_tokenizer_cache*')

    task_res['result'] = base64.b64encode(content).decode()
    return task_res
