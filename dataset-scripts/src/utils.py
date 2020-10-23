import os
import contextlib

@contextlib.contextmanager
def tmp_cwd(path):
    cwd = os.getcwd()
    try:
        os.chdir(path)
        yield
    finally:
        os.chdir(cwd)
