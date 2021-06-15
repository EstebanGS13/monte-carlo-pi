#!bin/bash

gcc dartboard_seq.c -o dart_seq
gcc -Wall -Ofast -funroll-loops dartboard_seq.c -o dart_seq_opt

mpicc dartboard_mpi.c -o dart_mpi
mpicc -Wall -Ofast -funroll-loops dartboard_mpi.c -o dart_mpi_opt

scp -r /home/cluser/monte-carlo cluser@wn1:/home/cluser/monte-carlo/
scp -r /home/cluser/monte-carlo cluser@wn2:/home/cluser/monte-carlo/
scp -r /home/cluser/monte-carlo cluser@wn3:/home/cluser/monte-carlo/

# MPI opt
for i in {1..5}; do
    for j in 2 4 10; do
        for k in {7..10}; do
            mpirun -np $j -hosts head,wn1,wn2,wn3 ./dart_mpi_opt $k
        done
    done
done
mv dartboard_mpi.csv dartboard_mpi_opt.csv

# MPI
for i in {1..5}; do
    for j in 2 4 10; do
        for k in {7..10}; do
            mpirun -np $j -hosts head,wn1,wn2,wn3 ./dart_mpi $k
        done
    done
done

# Seq opt
for i in {1..5}; do
    for j in {7..10}; do
        ./dart_seq_opt $j
    done
done
mv dartboard_seq.csv dartboard_seq_opt.csv

# Seq
for i in {1..5}; do
    for j in {7..10}; do
        ./dart_seq $j
    done
done
