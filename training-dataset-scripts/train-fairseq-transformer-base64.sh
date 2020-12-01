#!/bin/bash

#SBATCH -N 1  # number of nodes
#SBATCH -c 1  # number of "tasks" (cores)
#SBATCH --mem=16G        # GigaBytes of memory required (per node)
#SBATCH -t 0-12:00:00   # time in d-hh:mm:ss
##SBATCH -p rcgpu1,rcgpu6,cidsegpu1   # partition (not mrlinegpu1)
#SBATCH -p htcgpu,rcgpu1,rcgpu6,cidsegpu1   # partition (not mrlinegpu1)
#SBATCH --gres=gpu:2    # Request one GPU
#SBATCH -C V100      # Request GPU v100 with 32GB
#SBATCH -q wildfire     # QOS
#SBATCH -o slurm.%j.out # file to save job's STDOUT (%j = JobId)
#SBATCH -e slurm.%j.err # file to save job's STDERR (%j = JobId)

# Always purge modules to ensure consistent environments
module purge
# Load required modules for job's environment
module load anaconda/py3 clang/9.0.1 

# just to be sure

SCRATCH=/scratch/$USER
TYPE="$1"

pushd $SCRATCH/train-venv
source bin/activate

mail -s "[576-agave] Starting training" ${USER}@asu.edu << EOF
Starting training a transformer_from_pretrained_xlm model on the allstar-smoke dataset.
EOF

pushd fairseq
  # train the model
  mkdir -p $SCRATCH/smoke/transformer_from_xlm_old/checkpoints

  fairseq-train \
                  $SCRATCH/smoke/final/train-data/$TYPE  \
                  --arch transformer \
                  --optimizer adam --lr-scheduler reduce_lr_on_plateau \
                  --lr-shrink 0.5 --lr 1e-4  --min-lr 1e-09 --dropout 0.1 \
                  --fp16 \
                  --save-dir $SCRATCH/smoke/final/train-checkpoints/$TYPE \
                  --max-tokens 3072 \
                  --max-source-positions 3072 \
                  --max-target-positions 3072 \
                  --source-lang decomp --target-lang raw \
                  --skip-invalid-size-inputs-valid-test


mail -s "[576-agave] Training finished" ${USER}@asu.edu << EOF
Finished training a model on the allstar-smoke dataset.
EOF
