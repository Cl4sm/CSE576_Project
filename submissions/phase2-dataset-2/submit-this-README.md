# README

This is a document describing how we created the dataset, how to run the scripts to recreate the dataset and the task split up
between the team for the project. We strongly recommend using a Markdown previewer to read this, especially for the instructions to
recreate the dataset: use either your editor's markdown preview or [an online previewer](https://dillinger.io/).

## Dataset description

Our dataset is derived from [Allstar](https://allstar.jhuapl.edu), a public repository of Debian packages specially created
for reverse engineering research.

The dataset is a collection of source functions from various packages and their binaries. The source files are processed
and JSON files containing source level information about functions present in each package(the name of the source
file and the line numbers in the source file) are created. We create 1 JSON file for each package.

The dataset is created using 3 levels of filtering:

1. **Debian tags**: [Debian tags](https://wiki.debian.org/Debtags/FAQ) is a collection of metadata for each Debian package. While
   the actual vocabulary is quite detailed and extensive, we only seek to obtain the following information in the first level of
   classication:
   - What programming languages are used in the package?
   - Does the package contain a runnable program?

   Based on this information, we classify packages into following categories:

    1. Packages that don't contain a runnable program
    2. Package which may or may not contain a runnable program
    3. Runnable packages written in C
    4. Runnable packages with no C code at all
    5. Runnable packages partially written in C and partially some other language(s)
    6. Runnable packages whose programming language is unknown

    - Categories 1 and 4 are directly rejected because we are focusing only on C programs and our evaluation requires executables
      since we plan to run test suites to ensure semantic correctness is preserved.
    - Categories 2, 5 and 6 require further processing. For now, we will ignore 2 and perform the second filtering for other
      categories.
    - Category 3 is directly shortlisted for the final filtering.

2. **Linguist**: In this step, we use [GitHub's linguist](https://github.com/github/linguist) to determine the most common
   programming language in a package. If the most common language in the package is C, then we shortlist the package for the third
   level filtering.
3. **Allstar**: In this final filtering, we check the Allstar repository to see if there is an AMD64 executable present for the
   package. If there is none present, then we discard the package and do not add it to the dataset.

After this filtering, we have a collection of functions as well as the executables for the dataset. We have a total of 1127200 functions
across 2087 packages. Since our dataset would be extremely large(with both source files and binary executables), we are including
only the source level information of functions(compressed) derived from the source packages and binaries and only part
of the source code; the rest can be obtained from the public Allstar and Debian source package repositories.

We also expect additional filtering and processing of the dataset would become necessary as we make further progress on the
project and more aspects become clearer. We will add additional details in future updates about the project.

## Task split up

For the project, we split our initial tasks along these three categories:

1. Tokenizing source code and IDA decompiler output.
2. Creating and training the model using fairseq.
3. Creating the dataset.

We assigned a specific person responsible for each task who will perform the implementation. However, high level details of each
task are discussed among the team and finalized. Additionally, members also help the responsible person as needed. The task
responsibility assignment was done as follows:

- William Gibbs: Tokenizing source code using IDA decompiler output.
- Yihui Zeng: Tokenizing source code using clang and IDA decompiler output.
- Maxfield Lehman: Responsible for model creation and training using fairseq. Also helped with generating dataset.
- HuiJun Tay: Responsible for model creation and training using fairseq. Also helped with generating dataset.
- Arvind Sriram Raj: Creating initial dataset from which training and testing sets will be derived.

In addition, we will split the final evaluation task amongst all of us. We have high level evaluation tasks identified but
since specific details of those tasks have not yet been identified, we do not have a clear distribution of tasks amongst us yet.

## How to run the code

1. Setup a python virtual environment and install packages required for running the code. We use Ubuntu 18.04 LTS and the
   instructions are mostly based on that; please adjust accordingly for other distros:

   ```bash
   $ sudo apt update && sudo apt install python3-pip docker-ce
   $ sudo pip install virtualenvwrapper
   # Add "source /usr/local/bin/virtualenvwrapper.sh" to your shell's RC($HOME/.bashrc, $HOME/.zshrc etc) file
   # and restart terminal before continuing
   $ mkvirtualenv --python=`which python3` cse576
   (cse576) $ pip install -r requirements.txt
   ```
2. Build the dataset by running the automation script we have prepared. This script will perform several tasks
   - Parse debtags file into a TinyDB database
   - Perform first classification of packages using the `debtags` file
   - Download all source code in a docker environment(This step may take a long time, up to hours)
   - Perform second level classification using GitHub linguist.
   - Extract data using clang parser. This step would display errors that source packages could not be found for some packages. This
       is because we use a heuristic to find the source folder(search for folders named `src`, `Src`, `source` etc). We will explore
       improving this going ahead. Alternatively, you can manually specify the source directory name using `--src-dirs`.

   Before running the code, make sure you have a working docker environment installed
   ```bash
   python gen_dataset.py -o <output_folder>
   ```

3. Perform final filtering using the Allstar dataset. This uses a package name to package version mapping derived by
   parsing the debian packaging metadata(using the files `runnable-c-only-packages-names.txt` and
   `runnable-unknown-language-packages-names.txt`).

    ```bash
    $ workon cse576  # Activate virtual environment if not active
    (cse576) $ python3 allstar-filter-nobins.py debtags-classification/runnable-c-only-packages
    (cse576) $ python3 allstar-filter-nobins.py debtags-classification/runnable-unknown-language-packages
    ```
