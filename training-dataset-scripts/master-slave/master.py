"""
cloud master for parallel processing
"""
import os
import json
import logging
from threading import Lock

import colorlog
from flask import Flask, request

from utils import new_logger
from shared_func import get_all_items, process_task_result, populate

#####------initialization-----#####
logger = new_logger("MASTER")
logger.setLevel("INFO")

app = Flask(__name__)
app.extensions["lock"] = Lock()
app.extensions["all_tasks"] = []
app.extensions["processing_tasks"] = {}
app.extensions["completed_tasks"] = []
app.extensions["errored_tasks"] = []
app.extensions["next_task_id"] = 0
app.extensions["missed_tasks"] = []


#####------ views----------------------#####
@app.route('/')
def index():
    """
    Home handler
    """
    return "MASTER_SAYS_HELLO"

@app.route('/is_kill')
def is_exit():
    """
    kill switch
    """
    logger.info("[+] Completed: %d/%d",
                len(app.extensions["completed_tasks"]),
                len(app.extensions["all_tasks"]))
    # if you want to kill all the clients..
    # change this to KILL
    return "NOKILL"

@app.route('/get_task', methods=['GET'])
def server_get_next_task():
    """
    Get next request to process
    """
    task = get_next_task_act()
    if task is None:
        logger.warning("[+] No Task")
        return "NOTASK"
    logger.debug("[+] Sending Task: %s", task)
    # add task id to processing list
    add_processing_task(task)
    return json.dumps(task)

@app.route('/set_task_res', methods=['POST'])
def server_process_task_result():
    """
    process result of a task
    """
    task = json.loads(request.data.decode('utf8'))
    task_id = task['task_id'] if 'task_id' in task else None
    if 'err_msg' in task and task['err_msg']:
        logger.error("Error when processing result of task %d", task['task_id'])
        logger.error(task['err_msg'])

    if task_id is not None:
        logger.info("[+] Task finished for: %d", task_id)
        try:
            process_task_result(task)
            add_completed_task(task)
        except Exception as e:# pylint: disable=broad-except
            logger.error("Error when processing result of task %d", task['task_id'])
            logger.exception(e)
            add_errored_task(task)
        finally:
            remove_processing_task(task)
    else:
        logger.error("[-] Received Invalid task result")
        logger.error(request.data)

    return "ALLOK"

#####--------internal functions---------#####
def get_next_task_act():
    """
    get next task and transform the raw item to transfer-ready task
    """
    all_tasks = app.extensions["all_tasks"]
    missed_tasks = app.extensions["missed_tasks"]
    with app.extensions["lock"]:
        if missed_tasks:
            next_task = missed_tasks[0]
            app.extensions["missed_tasks"] = missed_tasks[1:]
            return next_task
        task_id = app.extensions["next_task_id"]
        if task_id >= len(all_tasks):
            return None
        next_task = all_tasks[task_id]
        app.extensions["next_task_id"] += 1
    return populate(next_task)

def remove_processing_task(task):
    """
    remove the task from processing list
    """
    processing_tasks = app.extensions["processing_tasks"]
    with app.extensions["lock"]:
        if task['task_id'] not in processing_tasks:
            logger.error("[!] Invalid Task Response received")
            logger.error("Processing: %s", processing_tasks)
            logger.error("Received Task ID: %d", task['task_id'])
        else:
            processing_tasks.pop(task['task_id'])

def add_processing_task(task):
    """
    add the task to processing list
    """
    processing_tasks = app.extensions["processing_tasks"]
    with app.extensions["lock"]:
        processing_tasks[task['task_id']] = task
        logger.debug('Processing len: %d', len(processing_tasks))
        logger.debug('Processing: %s', processing_tasks)

def add_completed_task(task):
    """
    add the task to completed task list
    """
    completed_tasks = app.extensions["completed_tasks"]
    with app.extensions["lock"]:
        completed_tasks.append(task)

def add_errored_task(task):
    """
    add the task to completed task list
    """
    completed_tasks = app.extensions["errored_tasks"]
    with app.extensions["lock"]:
        completed_tasks.append(task)
        logger.warning('Task %s Errored', task)

#####--------Customized API--------#####
@app.route('/save_state')
def save_state():
    """
    dump current state into a file
    """
    state = app.extensions.copy()
    state.pop('lock')
    with open(state_fname, "w") as f:
        json.dump(state, f)
    return 'OK'

if __name__ == '__main__':

    # parse arguments
    import argparse
    parser = argparse.ArgumentParser(description="Parallel processing - master server")
    parser.add_argument('-l', '--log-file', type=str, default="server_log.txt",
                        help="where to store the log file")
    parser.add_argument('-p', '--port', type=int, default=8080,
                        help="what port to bind")
    parser.add_argument('-R', '--resume', type=str, default="server_state.json",
                        help="resume the execution by providing the state.json")
    pargs = parser.parse_args()
    state_fname = pargs.resume

    # add file handler
    handler = logging.FileHandler(pargs.log_file, mode='a')
    formatter = colorlog.ColoredFormatter(
        '[%(name)s][%(levelname)s][%(asctime)s] %(log_color)s%(message)s',
        datefmt='%m-%d %H:%M:%S')
    handler.setFormatter(formatter)
    logging.root.addHandler(handler)

    # restore execution state
    if os.path.isfile(state_fname):
        with open(state_fname) as f:
            old_extensions = json.load(f)
        for key in old_extensions:
            app.extensions[key] = old_extensions[key]
        # move old processing_tasks and errored_tasks to missed_tasks
        app.extensions['missed_tasks'] += list(app.extensions['processing_tasks'].values())
        app.extensions['processing_tasks'] = {}
        app.extensions['missed_tasks'] += list(app.extensions['errored_tasks'])
        app.extensions['errored_tasks'] = {}

    if not app.extensions['all_tasks']:
        logger.info("Getting all tasks...")
        raw_tasks = get_all_items()
        app.extensions['all_tasks'] = [{'task_id': i, 'item': raw_tasks[i]}
                                       for i in range(len(raw_tasks))]
        save_state()

    logger.info("Master server started!")
    #app.run(host='0.0.0.0', debug=True, port=8080)
    app.run(host='0.0.0.0', port=pargs.port)
