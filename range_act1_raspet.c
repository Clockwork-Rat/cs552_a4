#include <stdio.h>
#include <unistd.h>
#include <mpi.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>



//compile
//mpicc -O3 range_query_starter.c -lm -o range_query_starter


//To run 
//srun range_query_starter 5000000 50000 ZTF_ra_dec_5m.csv


struct dataStruct
{
  double x;
  double y;
};

struct queryStruct
{
  double x_min;
  double y_min;
  double x_max;
  double y_max;
};

void generateQueries(struct queryStruct * data, unsigned int localQ, int my_rank);
int importDataset(char * fname, int N, struct dataStruct * data);


//Do not change constants
#define SEED 72
#define MINVALRA 0.0
#define MAXVALRA 360.0
#define MINVALDEC -40.0
#define MAXVALDEC 90.0

#define QUERYRNG 10.0 



int main(int argc, char **argv) {

  int my_rank, nprocs;

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  MPI_Comm_size(MPI_COMM_WORLD,&nprocs);

  //Process command-line arguments
  char inputFname[500];
  int N;
  int Q;
  

  if (argc != 4) {
    if(my_rank==0){
    fprintf(stderr,"Please provide the following on the command line: <Num data points in file> <Num query points> <Dataset file> %s\n",argv[0]);
    }
    MPI_Finalize();
    exit(0);
  }

  sscanf(argv[1],"%d",&N);
  sscanf(argv[2],"%d",&Q);
  strcpy(inputFname,argv[3]);
  

  
  const unsigned int localN=N;
  const unsigned int localQ=Q/nprocs;

  //local storage for the number of results of each query -- init to 0
  unsigned int * numResults=(unsigned int *)calloc(localQ, sizeof(unsigned int));

  //All ranks import dataset
  //allocate memory for dataset
  struct dataStruct * data=(struct dataStruct *)malloc(sizeof(struct dataStruct)*localN);

  int ret=importDataset(inputFname, localN, data);

  if (ret==1)
  {
    MPI_Finalize();
    return 0;
  }
  
  //All ranks generate different queries
  struct queryStruct * queries=(struct queryStruct *)malloc(sizeof(struct queryStruct)*localQ);
  generateQueries(queries, localQ, my_rank);

  MPI_Barrier(MPI_COMM_WORLD); 

  double stime = MPI_Wtime();

  //Write code here
  unsigned int global_count = 0;

  for (size_t n = 0; n < localN; ++n) {
    struct dataStruct point = data[n];
    for (size_t q = 0; q < localQ; ++q) {
      struct queryStruct query = queries[q];

      if (point.x >= query.x_min &&
          point.x <= query.x_max &&
          point.y >= query.y_min &&
          point.y <= query.y_max ) {
            ++numResults[q];
          } 
    }
  }

  double ttime = MPI_Wtime() - stime;
  double maxtime;

  MPI_Reduce(&ttime, &maxtime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
  MPI_Reduce(&numResults, &global_count, localQ, MPI_UNSIGNED, MPI_SUM, 0, MPI_COMM_WORLD);

  if (my_rank == 0) {
    printf("Sum: %u\n", global_count);
    printf("Total Time: %f\n", maxtime);
  }

  MPI_Finalize();
  return 0;
}



//generates queries
//x_min y_min are in [MINVALRA,MAXVALRA] + [MINVALDEC,MAXVALDEC]
//x_max y_max are MINVALRA+d1 MINVALDEC+d2
//distance (d1)= [0, QUERYRNG)
//distance (d2)= [0, QUERYRNG)
void generateQueries(struct queryStruct * data, unsigned int localQ, int my_rank)
{
  //seed rng do not modify
  //Different queries for each rank
  srand(SEED+my_rank);
  for (int i=0; i<localQ; i++)
  {
        data[i].x_min=MINVALRA+((double)rand()/(double)(RAND_MAX))*MAXVALRA;      
        //*2 because it starts at -90
        data[i].y_min=MINVALDEC+((double)rand()/(double)(RAND_MAX))*(MAXVALDEC*2); 
        
        double d1=((double)rand()/(double)(RAND_MAX))*QUERYRNG;      
        double d2=((double)rand()/(double)(RAND_MAX))*QUERYRNG;      
        data[i].x_max=data[i].x_min+d1;      
        data[i].y_max=data[i].y_min+d2;
  }
}


int importDataset(char * fname, int N, struct dataStruct * data)
{

    FILE *fp = fopen(fname, "r");

    if (!fp) {
        printf("Unable to open file\n");
        return(1);
    }

    char buf[4096];
    int rowCnt = 0;
    int colCnt = 0;
    while (fgets(buf, 4096, fp) && rowCnt<N) {
        colCnt = 0;

        char *field = strtok(buf, ",");
        double tmpx;
        sscanf(field,"%lf",&tmpx);
        // dataset[rowCnt][colCnt]=tmpx;
        data[rowCnt].x=tmpx;

        
        while (field) {
          colCnt++;
          field = strtok(NULL, ",");
          
          if (field!=NULL)
          {
          double tmpy;
          sscanf(field,"%lf",&tmpy);
          data[rowCnt].y=tmpy;
          }   

        }
        rowCnt++;
    }

    fclose(fp);

    return 0;


}


