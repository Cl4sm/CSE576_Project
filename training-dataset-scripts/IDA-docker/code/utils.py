import os
import logging
from contextlib import contextmanager

import tempfile
import colorlog

# color and format
logger_formatter = colorlog.ColoredFormatter(
    '[%(name)s][%(levelname)s]%(asctime)s %(log_color)s%(message)s',
    datefmt='%m-%d %H:%M')

def new_logger(name, level='DEBUG', new=True):
    # add custom level "VERBOSE"
    VERBOSE = 5
    logging.addLevelName(VERBOSE, "VERBOSE")
    logging.Logger.verbose = lambda inst, msg, *args, **kwargs: inst.log(VERBOSE, msg, *args, **kwargs)

    handler = logging.StreamHandler()
    handler.setLevel(level)
    handler.setFormatter(logger_formatter)

    logger = logging.getLogger(name)
    if new: logger.handlers = []

    logger.setLevel(logging.DEBUG)
    logger.addHandler(handler)

    return logger

@contextmanager
def path_context(path):
    cwd = os.getcwd()
    os.chdir(path)
    yield path
    os.chdir(cwd)

@contextmanager
def tmp_path(suffix=''):
    fpath = tempfile.mktemp(suffix=suffix, dir='/tmp')
    try:
        yield fpath
    finally:
        os.system(f"rm -rf {fpath}")
