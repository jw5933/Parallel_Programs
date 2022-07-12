#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <omp.h>

int bin(float f, float div){
    return (f == 100 ? (int)(f/div)-1 : (int)(f/div));
}

void printBins(int b, int *bins){
    int i;
    for (i = 0; i < b; i++){
        printf("bin[%d] = %d\n", i, bins[i]);
    }
}

int main(int argc, char *argv[]){
    int b, t;
    char filename[100]="";
    FILE * fp; //the file from input
    float *floats;
    int *bins;
    float binSize;
    int numOfFloats;
    int i;
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

    b = (unsigned int)atoi(argv[1]); 
    t = (unsigned int)atoi(argv[2]);
    strcpy(filename, argv[3]);

    if( !(fp = fopen(filename,"r")))
    {
        printf("Cannot open file %s\n", filename);
        exit(1);
    }

    //sequential
    fscanf(fp, "%d", &numOfFloats);

    floats = (float *)calloc(numOfFloats, sizeof(float));
    for (i = 0; i < numOfFloats; i++){
        fscanf(fp, "%f", &floats[i]);
    }
    fclose(fp);

    binSize = 100/b;
    //floats per bins -> 100/b
    //bins is an array of float arrays
    bins = (int*)calloc(b, sizeof(int));
    //limit of bin 0 = (0+1) * floats p bin
    t0 = omp_get_wtime();
    for (i = 0; i < numOfFloats; i++){
        bins[bin(floats[i], binSize)]++;
    }
    t1 = omp_get_wtime();

    printBins(b, bins);
    printf("time for calculation: %f\n", (t1 - t0));
}