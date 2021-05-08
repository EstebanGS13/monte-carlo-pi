#!bin/bash

gcc dartboard_seq.c -o dart_seq
gcc dartboard_threads.c -lpthread -o dart_th
gcc -Wall -Ofast -funroll-loops dartboard_threads.c -lpthread -o dart_th_off

for i in {1..5}; do
    for j in {7..10}; do
        ./dart_seq $j
    done
done

for i in {1..5}; do
    for j in {7..10}; do
        ./dart_th $j 4
    done
done
mv dartboard_threads.csv threads.csv

for i in {1..5}; do
    for j in {7..10}; do
        ./dart_th_off $j 4
    done
done
mv dartboard_threads.csv dartboard_optimized.csv
mv threads.csv dartboard_threads.csv
