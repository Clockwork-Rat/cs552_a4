#!/bin/bash
#SBATCH --job-name=a4-1-7
#SBATCH --output=/home/ijr44/cs552/a4/act1/run7.txt
#SBATCH --error=/home/ijr44/cs552/a4/act1/run7.err
#SBATCH --time=100:00
#SBATCH --mem=100G
#SBATCH --nodes=1
#SBATCH --ntasks=20
#SBATCH --cpus-per-task=1
#SBATCH --exclusive

module load openmpi

mpicc -O3 ~/cs552/cs552_a4/range_act1_raspet.c -lm -o ~/out_act1_7

srun ~/out_act1_7 5000000 50000 ~/ZTF_ra_dec_5m.csv