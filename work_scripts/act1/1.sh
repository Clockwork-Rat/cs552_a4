#!/bin/bash
#SBATCH --job-name=a4-1-1
#SBATCH --output=/home/ijr44/cs552/a4/act1/run1.txt
#SBATCH --error=/home/ijr44/cs552/a4/act1/run1.err
#SBATCH --time=100:00
#SBATCH --mem=100G
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --exclusive

module load openmpi

mpicc -O3 ~/cs552/cs552_a4/range_act1_raspet.c -lm -o ~/out_act1_1

srun ~/out_act1_1 5000000 50000 ZTF_ra_dec_5m.csv