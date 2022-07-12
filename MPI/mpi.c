// compile: mpicc  -std=c99 -Wall -o checkdiv mpi.c
// execute: mpiexec -n p ./checkdiv A B x

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <mpi.h>


int main(int argc, char *argv[]){
  
unsigned int x, A, B;
unsigned int i; //loop index
FILE * fp; //for creating the output file
char filename[100]=""; // the file name
unsigned int *numbers; //the numbers in the range [2, N]

double start_p1, end_p1, start_p2, end_p2;
double time_for_p1; 

int my_rank;   // process rank
int comm_sz; //number of processes
unsigned int *local_nums;


/////////////////////////////////////////


start_p1 = clock();
// Check that the input from the user is correct.
if(argc != 4){
  printf("usage:  ./checkdiv A B x\n");
  printf("A: the lower bound of the range [A,B]\n");
  printf("B: the upper bound of the range [A,B]\n");
  printf("x: divisor\n");
  exit(1);
}  

A = (unsigned int)atoi(argv[1]); 
B = (unsigned int)atoi(argv[2]); 
x = (unsigned int)atoi(argv[3]);
 

// The arguments to the main() function are available to all processes and no need to send them from process 0.
// Other processes must, after receiving the variables, calculate their own range.
MPI_Init(&argc, &argv);
MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);


/////////////////////////////////////////


/////////////////////////////////////////
//start of part 1
start_p1 = MPI_Wtime();
// The main computation part starts here
//calculate how many numbers processor is calculating locally
unsigned int nLength = B-A+1;
unsigned int curr_size = 0;
unsigned int local_length = nLength/comm_sz;
unsigned int my_start = my_rank * local_length;
unsigned int my_end = (my_rank == comm_sz-1) ? B-A+1 : my_start + local_length;
if (my_rank == comm_sz-1) local_length = my_end - my_start;

//allocate space according to rank. only process 0 needs full size, since all other processes will send their result to 0
if (my_rank == 0)
  numbers = malloc(nLength * sizeof(unsigned int));
else {
  local_nums = malloc(local_length * sizeof(unsigned int));
}

//data is not shared amongst processes
//find all numbers divisible by x
unsigned int next;
for (i = 0; i < local_length; i++){
  next = my_start + i + A;
  if (next % x == 0){
    if (my_rank == 0){
      numbers[curr_size] = next;
    }
    else{
      local_nums[curr_size] = next;
    }
    curr_size++;
  }
}

//recieve in order, to next open slot
//get size of sent data and adjust currsize accordingly
if (my_rank == 0){
  MPI_Status status;
  unsigned int max_size = (nLength / comm_sz) + (nLength % comm_sz);
  unsigned int size = 0;
  for (i = 1; i < comm_sz; i++){
    MPI_Recv(&numbers[curr_size], max_size, MPI_UNSIGNED, i, 0, MPI_COMM_WORLD, &status);
    MPI_Get_count(&status, MPI_UNSIGNED, &size);
    curr_size += size;
  }
}
else{
  MPI_Send(local_nums, curr_size, MPI_UNSIGNED, 0, 0, MPI_COMM_WORLD);
  free(local_nums);
}

// end of the main compuation part
end_p1 = MPI_Wtime();


// Use reduction operation to get MAX of (end_p1 - start_p1) among processes 
// and send it to process 0 as time_for_p1
// MPI_Reduce( void* send_data, void* recv_data, int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm communicator);
double total_p1 = end_p1 - start_p1;
MPI_Reduce(&total_p1, &time_for_p1, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
//end of part 1
/////////////////////////////////////////


/////////////////////////////////////////
//start of part 2
// Writing the results in the file


//forming the filename

//Write the numbers divisible by x in the file as indicated in the lab description.
if (my_rank == 0){
  start_p2 = MPI_Wtime();

  strcpy(filename, argv[2]);
  strcat(filename, ".txt");

  if( !(fp = fopen(filename,"w+t")))
  {
    printf("Cannot create file %s\n", filename);
    exit(1);
  }

  for (i = 0; i < curr_size; i++){
    fprintf(fp, "%d\n", numbers[i]);
  }

  fclose(fp);
  free(numbers);
  
  end_p2 = MPI_Wtime();
}

MPI_Finalize();

//end of part 2
/////////////////////////////////////////

/* Print  the times of the three parts */
if (my_rank == 0){
  printf("time of part1 = %lf s    part2 = %lf s\n", 
        (double)(time_for_p1),
        (double)(end_p2-start_p2) );
}
return 0;
}

