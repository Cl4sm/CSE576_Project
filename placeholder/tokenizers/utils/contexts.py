import os
from contextlib import contextmanager

@contextmanager
def cwd_cxt(wd):
    cwd = os.getcwd()
    try:
        os.chdir(wd)
        yield
    finally:
        os.chdir(cwd)
