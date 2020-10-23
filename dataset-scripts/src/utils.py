import os
import random
import string
import contextlib

import delegator

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

@contextlib.contextmanager
def git_cxt():
    git_commands = ["git init", "git add .", "git commit -m 'blah'"]
    try:
        os.system("rm -rf .git")

        # create a git context
        for command in git_commands:
            c = delegator.run(command)
            if c.return_code != 0:
                print("failed to create git repo")
                yield False
                return
        yield True
    finally:
        # cleanup
        os.system("rm -rf .git")
