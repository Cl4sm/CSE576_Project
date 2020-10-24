import os
from  parse_package_using_clang import main as clang_parse

BASE_DIR = "/home/kylebot/Desktop/courses/CSE576/src/CSE576_Project/dataset-scripts/output/dataset-packages/runnable-c-only-packages/"
package_names = [x for x in os.listdir(BASE_DIR) if os.path.isdir(os.path.join(BASE_DIR, x))]

for pn in package_names:
    try:
        clang_parse(pn, None, "./test", os.path.join(BASE_DIR, pn))
    except Exception:
        pass
