================================================
ABOUT
================================================
This lab generates a histogram given a series of p floating point numbers (under 100) from a file. There is a sequential version and a threaded version for analysis purposes.

Input: b t f
0 < b <= 50 	<- number of bins for histogram
0 < t <= 100 	<- number of threads
f 		<- filename

Output: histogram
on console.

example:
> ./histogram 10 20 nums.txt 
(10 bins from nums.txt file, performed w/ 20 threads)

================================================
HOW TO RUN
================================================
Compile:
gcc -fopenmp -Wall -std=c99 -o histogram openmp.c

Execute: 
./histogram b t f

================================================
OTHER
================================================
There is also included a file to generate floating points.

Input: f.txt n
f <- filename
n <- numbers to generate

gcc -o name.txt random-num.c
./nums f n