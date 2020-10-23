import os
import docker

from src.utils import tmp_cwd, docker_cleanup
from src.parse_debtags_to_tinydb import main as debtags_to_tinydb
from src.classify_packages_using_debtags import main as classify_packages
from src.filter_packages_using_github_linguist import main as gl_filter_packages
from src.parse_package_using_clang import main as clang_parse

import tqdm


##### CONFIG ######
DOCKER_IMAGE_NAME = "jessie-downloader"
DOCKERFILE_DIR = "downloader-docker"
PACKAGE_DIR = "dataset-packages"
DEBTAG_FILE = "debtags-2020-10-16.txt"
FILTER_TYPES = ["runnable-partially-c-packages", "runnable-unknown-language-packages"]
C_ONLY_TYPES = ["runnable-c-only-packages"]
GL_RES_DIR = "gl_results"
CLANG_RES_DIR = "clang_results"

##### INIT ######
client = None
db_path = None
package_dir = None
output_dir = None
gl_res_dir = None
clang_res_dir = None

##### CODE ######
def init(args):
    images = client.images.list(name=DOCKER_IMAGE_NAME)
    if len(images) < 1:
        print(f"Start building docker image {DOCKER_IMAGE_NAME}...")
        client.images.build(path=DOCKERFILE_DIR, tag=DOCKER_IMAGE_NAME)
        print(f"docker image {DOCKER_IMAGE_NAME} is built successfully!")

    if not os.path.exists(args.output_folder):
        os.mkdir(args.output_folder)

    if not os.path.exists(package_dir):
        os.mkdir(package_dir)

    if not os.path.exists(gl_res_dir):
        os.mkdir(gl_res_dir)

    if not os.path.exists(clang_res_dir):
        os.mkdir(clang_res_dir)

    print(f"Initialization success!")

def download_prep(debtag_file, output_dir):
    print("parse debtags file..")
    debtags_to_tinydb(debtag_file, db_path)

    print("classify packages..")
    classify_packages(db_path, package_dir)

def download():
    print('Start downloading packages. It may take hours...')
    volumes = {package_dir: {'bind': '/dataset-packages/', 'mode': 'rw'}}

    with docker_cleanup(client) as tmp_name:
        container = client.containers.run(DOCKER_IMAGE_NAME, auto_remove=True, volumes=volumes, name=tmp_name, detach=True)

        # steam the output
        it = container.logs(stream=True)
        try:
            while container.status == 'created':
                print(it.next().decode().strip())
        except Exception:
            pass

def do_gl_filtering():
    print("Starting filtering packages using github linguist...")
    for ftype in FILTER_TYPES:
        filtered_res = os.path.join(gl_res_dir, ftype+"-filtered.txt")
        type_dir = os.path.join(package_dir, ftype)
        gl_filter_packages(type_dir, filtered_res)

def do_clang_parse_one_package(ftype, package_name):
    output_path = os.path.join(clang_res_dir, package_name+".json")
    package_tree = os.path.join(package_dir, ftype, package_name)
    assert os.path.isdir(package_tree)

    clang_parse(package_name, None, output_path, package_tree)

def gen_task_for_clang():
    for ftype in C_ONLY_TYPES:
        ftype_dir = os.path.join(package_dir, ftype)
        package_names = [x for x in os.listdir(ftype_dir) if os.path.isdir(os.path.join(ftype_dir, x))]
        for package_name in package_names:
            yield (ftype, package_name)

    for ftype in FILTER_TYPES:
        try:
            filtered_res = os.path.join(gl_res_dir, ftype+"-filtered.txt")
            with open(filtered_res) as f:
                content = f.read()
        except Exception:
            continue
        package_names = [x.strip() for x in content.splitlines() if x]
        for package_name in package_names:
            yield (ftype, package_name)

def do_clang_parse():
    print("Starting parsing packages using libclang...")
    gen = gen_task_for_clang()

    tasks = list(gen)

    for task in tqdm.tqdm(tasks, desc="clang-parse"):
        ftype = task[0]
        package_name = task[1]
        do_clang_parse_one_package(ftype, package_name)


if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument("-o", "--output-folder", type=str, required=True, help="the output folder")
    parser.add_argument("-d", "--debtag", type=str, default=DEBTAG_FILE, help="the debtag file, something like 'debtags-2020-10-16.txt'")
    parser.add_argument("--init-only", default=False, action="store_true", help="do we want to stop after initialization")
    parser.add_argument("--download-only", default=False, action="store_true", help="do we want to stop after downloading the packages")
    parser.add_argument("-no-dl", "--skip-download", default=False, action="store_true", help="if the packages are downloaded already, we can skip it")
    args = parser.parse_args()

    # initialize variables
    client = docker.from_env()
    db_name = os.path.basename(args.debtag).strip(".txt") + ".json"
    db_path = os.path.abspath(os.path.join(args.output_folder, db_name))
    package_dir = os.path.abspath(os.path.join(args.output_folder, PACKAGE_DIR))
    output_dir = os.path.abspath(args.output_folder)
    gl_res_dir = os.path.abspath(os.path.join(output_dir, GL_RES_DIR))
    clang_res_dir = os.path.abspath(os.path.join(output_dir, CLANG_RES_DIR))

    # perform initialization
    init(args)
    if args.init_only:
        exit(0)

    # now download packages
    if not args.skip_download:
        download_prep(args.debtag, args.output_folder)
        download()
    if args.download_only:
        exit(0)

    # filter downloaded packages by using "github linguist"
    do_gl_filtering()

    # parse packages using clang
    do_clang_parse()

