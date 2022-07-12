#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <omp.h>

//gcc -fopenmp -Wall -std=c99 -o histogram jw5933.c
int getBin(float f, float div){
    return (f == 100 ? (int)(f/div)-1 : (int)(f/div));
}

    int main(int argc, char *argv[]){
    FILE * fp; //the file from input
    unsigned int num_of_floats;
    double t0, t1;

    /////////////////////////////////////////
    // Check input
    if(argc != 4){
    printf("usage:  ./histogram b t filename\n");
    printf("b: the number of bins\n");
    printf("t: the number of threads\n");
    printf("filename: the name of the text file that contains the floating point numbers\n");
    exit(1);
    }  

    unsigned int b = (unsigned int)atoi(argv[1]); 
    unsigned int t = (unsigned int)atoi(argv[2]);
    char *filename = argv[3];

    if( !(fp = fopen(filename,"r")))
    {
        printf("Cannot open file %s\n", filename);
        exit(1);
    }

    //make reading and calculations parrallel?
    fscanf(fp, "%d", &num_of_floats);

    float *floats = (float *)calloc(num_of_floats, sizeof(float));
    for (unsigned int i = 0; i < num_of_floats; i++){
        fscanf(fp, "%f", &floats[i]);
    }
    fclose(fp);

    float bin_size = 100/b;

    //floats per bins -> 100/b
    //bins is an array of float arrays
    unsigned int *bins = (unsigned int*)calloc(b, sizeof(unsigned int));
    //limit of bin 0 = (0+1) * floats p bin

    //when the bins are done calcuating, threads should add all the numbers unsigned int one bin
    //-> they need access to other bins local bin
    //an array of bins unsigned int each thread
    unsigned int **local_bins = (unsigned int **)calloc(t, sizeof(unsigned int*));
    
    t0 = omp_get_wtime();
#   pragma omp parallel num_threads(t)
    {
        int thread_rank = omp_get_thread_num();
        local_bins[thread_rank] = (unsigned int *)calloc(b, sizeof(unsigned int));

#       pragma omp for
        //each thread calculates its own bin from its given set of floats
        for(unsigned int local_i = 0; local_i < num_of_floats; local_i++){
            local_bins[thread_rank][getBin(floats[local_i], bin_size)]++;
        }

        //need the bins to be done calculating, and when done adding can just leave
#       pragma omp for
        for (unsigned int b_num = 0; b_num < b; b_num++){
            //threads are distributed a bin
            //add all local bins for that bin num
            for (unsigned int t_num = 0; t_num < t; t_num++){
                bins[b_num] += local_bins[t_num][b_num];
            }
        }
    }
    t1 = omp_get_wtime();
    free(floats);
    for (unsigned int i = 0; i < t; i++){
        free(local_bins[i]);
    }
    free(local_bins);

    //printing must happen at the very end
    for (unsigned int i = 0; i < b; i++){
        printf("bin[%d] = %d\n", i, bins[i]);
    }
    free(bins);

    printf("time for parallel: %f\n", (t1 - t0));
    return 0;
}