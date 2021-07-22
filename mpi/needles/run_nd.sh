#!bin/bash

gcc needles_seq.c -o nd_seq -lm
gcc -Wall -Ofast -funroll-loops needles_seq.c -o nd_seq_opt -lm

mpicc needles_mpi.c -o nd_mpi -lm

scp -r /home/cluser/needles cluser@wn2:/home/cluser/needles/
scp -r /home/cluser/needles cluser@wn3:/home/cluser/needles/
scp -r /home/cluser/needles cluser@wn1:/home/cluser/needles/

# MPI
for i in {1..5}; do
    for j in 2 4 10; do
        for k in {6..9}; do
            mpirun -np $j -hosts head,wn1,wn2,wn3 ./nd_mpi $k
        done
    done
done

# Seq opt
for i in {1..5}; do
    for j in {6..9}; do
        ./nd_seq_opt $j
    done
done
mv needles_seq.csv needles_seq_opt.csv

# Seq
for i in {1..5}; do
    for j in {6..9}; do
        ./nd_seq $j
    done
done
