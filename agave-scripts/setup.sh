#!/bin/bash

#SBATCH -N 1  # number of nodes
#SBATCH -c 4  # number of "tasks" (cores)
#SBATCH --mem=8G       # GigaBytes of memory required (per node)
# #SBATCH -p gpu
# #SBATCH -q wildfire
# #SBATCH --gres=gpu:1
# #SBATCH -C V100_16
#SBATCH -t 0-01:30:00   # time in d-hh:mm:ss
#SBATCH -o slurm.%j.out # file to save job's STDOUT (%j = JobId)
#SBATCH -e slurm.%j.err # file to save job's STDERR (%j = JobId)

# Handle module dependencies
## Always purge modules to ensure consistent environments
module purge
## Load required modules for job's environment
module load anaconda/py3 gcc/8.4.0

SCRATCH=/scratch/$USER/decompiler-project
mkdir -p $SCRATCH
pushd $SCRATCH
  # Install fairseq/sacremoses/apex in a new virtualenv
  venv="$SCRATCH/dec-raw-venv"
  test -d "$venv" || virtualenv --no-site-packages "$venv"
  pushd "$venv"
    source bin/activate

    # we need up-to-date pip to install them without errors for some reason
    pip install -U pip

    # install fairseq
    if [ ! -d fairseq ]
    then
      git clone https://github.com/pytorch/fairseq
    fi
    pushd fairseq
      pip install --editable ./
    popd

    pip install sacremoses

    # install apex
    if [ ! -d apex ]
    then
      git clone https://github.com/NVIDIA/apex
    fi
    pushd apex
      pip install --global-option="--cpp_ext" \
                  --global-option="--cuda_ext" \
                  --global-option="--deprecated_fused_adam" \
                  --global-option="--xentropy" \
                  --global-option="--fast_multihead_attn" \
                  .
    popd

  if [ ! -d "$SCRATCH/smoke/fairseq_dataset"]
  then
    rsync -rv --append /scratch/mclehman/smoke/fairseq_dataset $SCRATCH/smoke/fairseq_dataset
  fi

  # Install our code into the venv
  remote_repo="/scratch/mclehman/decompiler-project-phase3"
  repo="$SCRATCH/decompiler-project-phase3"
  if [ ! -d "$SCRATCH/decompiler-project-phase3"]
  then
    rsync -rv --append $remote_repo $repo
    pushd $repo
      pip install .
    popd
  fi

  # Copy custom tokenizer into fairseq
  script_dir="/scratch/mclehman/scripts"
  cp $script_dir/clang_tokenizer.py $venv/fairseq/fairseq/data/encoders/
popd
