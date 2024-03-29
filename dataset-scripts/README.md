# Dataset parser

Scripts and other files used for building the dataset from Debian packages.

- `debtags-YYYY-MM-DD.txt`: Copy of debtags as of YYYY-MM-DD downloaded from [here](https://debtags.debian.org/exports/stable-tags).
- `parse-debtags-to-tinydb.py`: Parse debtags into [TinyDB](https://tinydb.readthedocs.io/) database.
- `classify-packages-using-debtags.py`: Level 1 package classification using debtags.
- `find-c-packages.py`: Level 2 classification using [GitHub linguist](https://github.com/github/linguist).
- `parse-package-using-clang.py`: Level 3 classification using [libclang](https://pypi.org/project/libclang/).
- `allstar.py`: Allstar script for pulling from their report from [here] (https://github.com/JHUAPL/ALLSTAR/blob/master/allstar.py).
- `allstar-filter-nobins.py`: filter out allstar packages with no corresponding binaries on their repo.
