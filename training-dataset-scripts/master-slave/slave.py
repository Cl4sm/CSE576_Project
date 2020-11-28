"""
slave script to process tasks
"""
import os
import sys
import time
import json
import logging

import requests
import colorlog

from utils import new_logger
from shared_func import process_task

#####---------Configuration-----------#####
NO_TASK_STR = "NOTASK"

#####---------Initialization----------#####
logger = new_logger("SLAVE")
logger.setLevel("INFO")


#####---------Internal Functions-------#####
def get_next_task():
    """
    retrieve next task
    """
    try:
        r = requests.get(GET_REQ_URL)
        return json.loads(r.text)
    except Exception as e:# pylint: disable=broad-except
        if r.text != NO_TASK_STR:
            logger.exception(e)
        return None

def is_kill():
    """
    check server status
    """
    try:
        r = requests.get(KILL_SWITCH)
        return str(r.text) == "KILL"
    except Exception as e:# pylint: disable=broad-except
        logger.exception(e)
        return None

def send_response(task):
    """
    send task result back to master
    """
    headers = {'Content-type': 'application/json', 'Accept': 'text/plain'}
    logger.info("[+] Sending Response back to server.")
    logger.debug(task)
    r = requests.post(SEND_RES_URL, data=json.dumps(task), headers=headers)
    logger.debug("[+] Server Says: %s", r.text)

def master_save_state():
    """
    make server save its state
    """
    requests.get(SAVE_STATE_URL)

#####--------Main Loop----------#####
def main():
    """
    main loop
    """
    while True:
        try:
            # do different tasks according to master's status
            kill_res = is_kill()
            if kill_res is None:
                # Server not up.
                # sleep for 5 seconds
                logger.info("[+] Sleeping")
                time.sleep(5)
                continue
            if kill_res:
                # Killed
                logger.info("[*] Killed.")
                break
            # not killed?
            # get next task to process
            task = get_next_task()
            master_save_state()

            if task is None:
                logger.info("[*] No requests to process.")
                break
            task['result'] = None
            task['err_msg'] = None

            # process the task and send the response back to master
            task = process_task(task)
            send_response(task)
            master_save_state()

        except OSError as e:
            logger.error("OSError when processing %s", task)
            logger.exception(e)
            logger.error("Exiting because of the OSError")
            exit()
        except Exception as e:# pylint: disable=broad-except
            logger.error("Error when processing %s", task)
            logger.exception(e)
            logger.info("[+] Sleeping")
            task['err_msg'] = str(e)
            send_response(task)
            time.sleep(5)


if __name__ == '__main__':
    # parse arguments
    import argparse
    parser = argparse.ArgumentParser(description="Parallel processing - slave host")
    parser.add_argument('-l', '--log-file', type=str, default=None,
                        help="where to store the log file")
    parser.add_argument('-H', '--host', type=str, default=None,
                        help="master ip")
    parser.add_argument('-p', '--port', type=int, default=None,
                        help="master port")
    pargs = parser.parse_args()

    # check log file
    if not pargs.log_file:
        print("Log file should be provided")
        exit()
    # check master
    if not pargs.host or not pargs.port:
        print("host and port should be provided")
        exit()

    # add file handler
    handler = logging.FileHandler(pargs.log_file, mode='a')
    formatter = colorlog.ColoredFormatter(
        '[%(name)s][%(levelname)s][%(asctime)s] %(log_color)s%(message)s',
        datefmt='%m-%d %H:%M:%S')
    handler.setFormatter(formatter)
    logging.root.addHandler(handler)

    srv_url = "http://%s:%d" % (pargs.host, pargs.port)
    GET_REQ_URL = srv_url + '/get_task'
    SEND_RES_URL = srv_url + '/set_task_res'
    KILL_SWITCH = srv_url + '/is_kill'
    SAVE_STATE_URL = srv_url + '/save_state'
    main()
