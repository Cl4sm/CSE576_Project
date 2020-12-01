#!/bin/bash

#SBATCH -N 1  # number of nodes
#SBATCH -c 1  # number of "tasks" (cores)
#SBATCH --mem=16G        # GigaBytes of memory required (per node)
#SBATCH -t 0-02:00:00   # time in d-hh:mm:ss
#SBATCH -p gpu   # partition (not mrlinegpu1)
#SBATCH --gres=gpu:1    # Request one GPU
##SBATCH -C V100         # Request GPU v100 with 32GB
#SBATCH -q wildfire     # QOS
#SBATCH -o slurm.%j.out # file to save job's STDOUT (%j = JobId)
#SBATCH -e slurm.%j.err # file to save job's STDERR (%j = JobId)

# Always purge modules to ensure consistent environments
module purge
# Load required modules for job's environment
module load anaconda/py3 clang/9.0.1 

SCRATCH=/scratch/$USER
TYPE="$1"

#activate virtualenv
pushd $SCRATCH/train-venv
source bin/activate

fairseq-generate \
                  $SCRATCH/smoke/final/train-data/$TYPE  \
                  --path $SCRATCH/smoke/final/eval/$TYPE/checkpoint_best.pt \
         	      --results-path $SCRATCH/smoke/final/eval/$TYPE \
                  --max-tokens 3072 \
                  --source-lang decomp --target-lang raw \
                  --max-source-positions 3072 --max-target-positions 3072 \
                  --skip-invalid-size-inputs-valid-test
