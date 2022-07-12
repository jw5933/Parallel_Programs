================================================
ABOUT
================================================
This lab discovers all the numbers divisible by x, within a range A - B, where x is a positive number between 2 and 1 000 000, and A, B are greater than 1 and less than or equal to 100 000 000.

Input: A B x
1 < A, B <= 100000000

Output: B.txt
all numbers between A, B (inclusive) that is divisible by x.

example:
> ./checkdiv 1 10 3
10.txt contains
3
6
9

================================================
# HOW TO RUN
================================================
Compile:
mpicc -std=c99 -Wall -o checkdiv mpi.c

Execute: 
mpiexec -n p ./checkdiv A B x

where 
p <- number of processes

example: 
mpiexec -n 8 ./checkdiv 2 100000000 91