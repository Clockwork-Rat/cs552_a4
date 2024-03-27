#!/bin/bash
#SBATCH --job-name=a4-3-7
#SBATCH --output=/home/ijr44/cs552/a4/act3/run7.txt
#SBATCH --error=/home/ijr44/cs552/a4/act3/run7.err
#SBATCH --time=100:00
#SBATCH --mem=100G
#SBATCH --nodes=2
#SBATCH --ntasks=20
#SBATCH --cpus-per-task=1
#SBATCH --exclusive
#SBATCH --ntasks-per-node=10

module load openmpi

mpic++ -O3 ~/cs552/cs552_a4/range_act2_raspet.cpp -lm -o ~/out_act3_7

srun ~/out_act3_7 5000000 50000 ~/ZTF_ra_dec_5m.csv