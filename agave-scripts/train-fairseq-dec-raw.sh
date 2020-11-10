#!/bin/bash

#SBATCH -N 1            # number of nodes
#SBATCH -c 1            # number of "tasks" (cores)
#SBATCH --mem=32G       # GigaBytes of memory required (per node)
#SBATCH -t 0-8:00:00    # time in d-hh:mm:ss
#SBATCH -p gpu          # partition (not mrlinegpu1)
#SBATCH --gres=gpu:2    # Request one GPU
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

TEXT=$SCRATCH/smoke/fairseq_dataset
rm -rf $SCRATCH/smoke/data-bin/allstar.decomp-raw
mkdir -p $SCRATCH/smoke/data-bin/allstar.decomp-raw

fairseq-preprocess \
                --source-lang decomp --target-lang raw \
                --trainpref $TEXT/train --validpref $TEXT/valid --testpref $TEXT/test \
                --destdir $SCRATCH/smoke/data-bin/allstar.decomp-raw \
                --tokenizer clangtok \
                --thresholdtgt 10 --thresholdsrc 10 \
                --workers 20

mail -s "[576-agave] Preprocessing finished" ${USER}@asu.edu << EOF
Finished training a model on the allstar-smoke dataset.
EOF

# train the model
mkdir -p $SCRATCH/smoke/checkpoints

fairseq-train \
                $SCRATCH/smoke/data-bin/allstar.decomp-raw  \
                --arch transformer  --share-decoder-input-output-embed \
                --optimizer adam --lr-scheduler reduce_lr_on_plateau \
                --lr 1e-4 --lr-scheduler inverse_sqrt \
                --save-dir $SCRATCH/smoke/checkpoints \
                --max-tokens 2048 --update-freq 8 \
                --tokenizer clangtok \
                --fp16 \
                --skip-invalid-size-inputs-valid-test \
                --max-epoch 200

mail -s "[576-agave] Training finished" ${USER}@asu.edu << EOF
Finished training a model on the allstar-smoke dataset.
EOF
