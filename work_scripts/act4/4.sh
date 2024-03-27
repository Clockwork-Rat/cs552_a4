#!/bin/bash
#SBATCH --job-name=a4-4-3
#SBATCH --output=/home/ijr44/cs552/a4/act4/run3.txt
#SBATCH --error=/home/ijr44/cs552/a4/act4/run3.err
#SBATCH --time=100:00
#SBATCH --mem=100G
#SBATCH --nodes=2
#SBATCH --ntasks=4
#SBATCH --cpus-per-task=1
#SBATCH --exclusive

module load openmpi

mpic++ -O3 ~/cs552/cs552_a4/range_act2_raspet.cpp -lm -o ~/out_act4_3

srun ~/out_act4_3 5000000 50000 ~/ZTF_ra_dec_5m.csv