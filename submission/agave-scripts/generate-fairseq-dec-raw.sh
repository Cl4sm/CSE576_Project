#!/bin/bash

#SBATCH -N 1            # number of nodes
#SBATCH -c 1            # number of "tasks" (cores)
#SBATCH --mem=32G       # GigaBytes of memory required (per node)
#SBATCH -t 0-04:00:00   # time in d-hh:mm:ss
#SBATCH -p gpu          # partition (not mrlinegpu1)
#SBATCH --gres=gpu:4    # Request one GPU
#SBATCH -C V100         # Request GPU v100 with 32GB
#SBATCH -q wildfire     # QOS
#SBATCH -o slurm.%j.out # file to save job's STDOUT (%j = JobId)
#SBATCH -e slurm.%j.err # file to save job's STDERR (%j = JobId)

# Always purge modules to ensure consistent environments
module purge
# Load required modules for job's environment
module load anaconda/py3 clang/9.0.1

SCRATCH="/scratch/$USER/decompiler-project"
venv="$SCRATCH/dec-raw-venv"

# activate virtualenv
source ${venv}/bin/activate

fairseq-generate \
        $SCRATCH/smoke/data-bin/allstar.decomp-raw \
        --path $SCRATCH/smoke/draft/checkpoint_best.pt \
        --results-path $SCRATCH/smoke/draft/results \
        --max-tokens 2048 \
        --fp16 --unkpen 10 \
        --num-workers 10 \
        --skip-invalid-size-inputs-valid-test
        # --tokenizer clangtok
