#!/bin/bash
#SBATCH --job-name=a4-3-4
#SBATCH --output=/home/ijr44/cs552/a4/act2/run4.txt
#SBATCH --error=/home/ijr44/cs552/a4/act2/run4.err
#SBATCH --time=100:00
#SBATCH --mem=100G
#SBATCH --nodes=2
#SBATCH --ntasks=8
#SBATCH --cpus-per-task=1
#SBATCH --exclusive
#SBATCH --ntasks-per-node=4

module load openmpi

mpic++ -O3 ~/cs552/cs552_a4/range_act2_raspet.cpp -lm -o ~/out_act3_4

srun ~/out_act3_4 5000000 50000 ~/ZTF_ra_dec_5m.csv