#!/bin/bash

#SBATCH -N 1  # number of nodes
#SBATCH -c 4  # number of "tasks" (cores)
#SBATCH --mem=32G        # GigaBytes of memory required (per node)
#SBATCH -t 0-1:00:00   # time in d-hh:mm:ss
#SBATCH -o slurm.%j.out # file to save job's STDOUT (%j = JobId)
#SBATCH -e slurm.%j.err # file to save job's STDERR (%j = JobId)

# Always purge modules to ensure consistent environments
module purge
# Load required modules for job's environment
module load anaconda/py3 clang/9.0.1 

SCRATCH=/scratch/$USER
pushd $SCRATCH
TYPE="$1"

mkdir -p venvs
pushd venvs

node_group=$(uname -n | sed 's/-.*//')
node_env="${node_group}-env"

# need a virtualenv to install fairseq/apex
test -d "$node_env" || virtualenv --no-site-packages "$node_env"
pushd "$node_env"
  source bin/activate

  # we need up-to-date pip to install them for some reason
  pip install -U pip

  # install fairseq
  if [ ! -d fairseq ]
  then
    git clone https://github.com/pytorch/fairseq
    pushd fairseq
      pip install --editable ./
    popd
  fi

  pip install sacremoses

  # install apex
  if [ ! -d apex ]
  then
    git clone https://github.com/NVIDIA/apex
    pushd apex
      pip install --global-option="--cpp_ext" \
                  --global-option="--cuda_ext" \
                  --global-option="--deprecated_fused_adam" \
                  --global-option="--xentropy" \
                  --global-option="--fast_multihead_attn" \
                  .
    popd
  fi

pushd $node_env
source bin/activate

cp /scratch/mclehman/cross_lingual_lm.py $node_env/fairseq/fairseq/tasks/cross_lingual_lm.py

pushd fairseq
  TEXT=$SCRATCH/smoke/final/raw-data/$TYPE
  DEST=$SCRATCH/smoke/final/train-data/$TYPE
  rm -rf $DEST
  mkdir -p $DEST

  fairseq-preprocess \
                  --source-lang decomp --target-lang raw \
                  --trainpref $TEXT/train --validpref $TEXT/valid --testpref $TEXT/test \
                  --destdir $DEST \
                  --thresholdtgt 2 --thresholdsrc 2 \
                  --workers 10
popd

pushd $DEST
  /scratch/mclehman/pipeline-scripts/dict-merge.sh dict.decomp.txt dict.raw.txt) > dict.txt
popd
