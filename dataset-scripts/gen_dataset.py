import os
import docker

from src.utils import tmp_cwd, docker_cleanup
from src.parse_debtags_to_tinydb import main as debtags_to_tinydb
from src.classify_packages_using_debtags import main as classify_packages


##### CONFIG ######
DOCKER_IMAGE_NAME = "jessie-downloader"
DOCKERFILE_DIR = "downloader-docker"
PACKAGE_DIR = "dataset-packages"
DEBTAG_FILE = "debtags-2020-10-16.txt"

##### INIT ######
client = None
db_path = None
package_dir = None

##### CODE ######
def init(args):
    images = client.images.list(name=DOCKER_IMAGE_NAME)
    if len(images) < 1:
        print(f"Start building docker image {DOCKER_IMAGE_NAME}...")
        client.images.build(path=DOCKERFILE_DIR, tag=DOCKER_IMAGE_NAME)
        print(f"docker image {DOCKER_IMAGE_NAME} is built successfully!")

    if not os.path.exists(args.output_folder):
        os.mkdir(args.output_folder)

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

if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument("-o", "--output-folder", type=str, required=True, help="the output folder")
    parser.add_argument("-d", "--debtag", type=str, default=DEBTAG_FILE, help="the debtag file, something like 'debtags-2020-10-16.txt'")
    parser.add_argument("--init-only", default=False, action="store_true", help="do we want to stop after initialization")
    parser.add_argument("--download-only", default=False, action="store_true", help="do we want to stop after downloading the packages")
    args = parser.parse_args()

    # initialize variables
    client = docker.from_env()
    db_name = os.path.basename(args.debtag).strip(".txt") + ".json"
    db_path = os.path.abspath(os.path.join(args.output_folder, db_name))
    package_dir = os.path.abspath(os.path.join(args.output_folder, PACKAGE_DIR))

    # perform initialization
    init(args)
    if args.init_only:
        exit(0)

    # now download packages
    download_prep(args.debtag, args.output_folder)
    download()
    if args.download_only:
        exit(0)
    

    #print(args)
    #if not os.path.exists(os.package_folder):
    #    pass



