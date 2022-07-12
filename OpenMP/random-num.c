#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(int argc, char *argv[])
{
    int count = 0;
    FILE * fp;
    float num = 100.0;
    
    if(argc != 3 ){
	printf("usage: ./genfile name num\n");
	printf("name: the name of the file to be generated\n");
	printf("num: the number of floating points to be generated\n");
 	exit(1);
    }

    if( (fp = fopen(argv[1],"w+t")) == NULL )
    {
	printf("Cannot create the file %s\n", argv[1]);
	exit(1);
    }

    count = atoi(argv[2]);

    srand((unsigned int)time(NULL));

    fprintf(fp,"%d ", count); 
    for (int i=0;i<count;i++)
        fprintf(fp, "%f ", ((float)rand()/(float)(RAND_MAX)) * num);
    
    printf("Generated %d floating point numbers and stored in file %s\n", count, argv[1]);

    fclose(fp);
    return 0;
}
