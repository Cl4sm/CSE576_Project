import os
import random
import string
import contextlib

@contextlib.contextmanager
def tmp_cwd(path):
    cwd = os.getcwd()
    try:
        os.chdir(path)
        yield
    finally:
        os.chdir(cwd)

@contextlib.contextmanager
def docker_cleanup(client):
    name = ''.join(random.choices(string.ascii_letters, k=20))
    try:
        yield name
    finally:
        client.containers.get(name).kill()
