#include <iostream>
#include <mpi.h>
using namespace std;

int main(){
   //Defining variables "rank" for the processor number and
   //size for the total number of processors to be used.
   int rank, size;

   //Defining the matrix A outside of the parallel environment.
   //This will therefore take the same value within each process.
   float Amatrix[3][3] = {
      {4, 3, 2},
      {10, 8, 6},
      {18, 15, 12}};

   //Initiating a matrix D = AB.
   float Dmatrix[3][3] = {0};

   //Initiating a long array for the gathered data,
   //the vector c from each process will be entered,
   //consecutively in order of rank.
   float Gathered_data[9] = {0};

   //Defining an MPI communicator, the communicator type
   //will be specified below.
   MPI_Comm comm;

   //Choosing the default communicator over all processors.
   comm  = MPI_COMM_WORLD;

   //Initialising parallel.
   MPI_Init(NULL,NULL);
   MPI_Comm_rank(comm, &rank);
   MPI_Comm_size(comm, &size);

   //Initialising the matrix B in all processors
   float Bmatrix[3][3];

   //Initialising a row of matrix A in all processors
   float Arow[3];

   //Initialising a vector c in all processors
   float cvector[3];

   //If the rank is 0 then generate the matrix B
   if (rank == 0){
      Bmatrix[0][0] = 6;
      Bmatrix[0][1] = 6;
      Bmatrix[0][2] = 4;
      Bmatrix[1][0] = 9;
      Bmatrix[1][1] = 8;
      Bmatrix[1][2] = 5;
      Bmatrix[2][0] = 12;
      Bmatrix[2][1] = 10;
      Bmatrix[2][2] = 6;

   }

   //Broadcasting the matrix B to all other processors.
   MPI_Bcast(&Bmatrix, 9, MPI_FLOAT, 0, comm);

   //Waiting until the broadcast is completed before proceeding.
   MPI_Barrier(comm);

   //Generating the corresponding row of A in each processor.
   Arow[0] = Amatrix[rank][0];
   Arow[1] = Amatrix[rank][1];
   Arow[2] = Amatrix[rank][2];

   //Computing the dot product of the row of A with
   //the matrix B to get the vector c of each processor.
   for (int column = 0; column < 3; column++){
      for (int row = 0; row < 3; row++){
         cvector[column] = cvector[column] + Arow[row] * Bmatrix[row][column];
      }
   }

   //Waiting until each cvector has been computed before proceeding.
   MPI_Barrier(comm);

   //Printing the c vector one by one for each process

   //processor 0
   if (rank == 0){

      cout << "I am processor "<< rank << " my vector c reads as follows:" << endl;

      for (int column = 0; column < 3; column++){
         cout << cvector[column] << " ";
      }
      cout << endl;

      cout << "I am processor "<< rank << " my row of A reads as follows:" << endl;
      for (int column = 0; column <3; column++){
         cout << Arow[column] << " ";
      }
      cout << endl;

      cout << "I am processor "<< rank << " my matrix B reads as follows:" << endl;
      for (int row = 0; row < 3; row++){
         for (int column = 0; column < 3; column++){
            cout << Bmatrix[row][column] << " ";
	 }
         cout << endl;
      }
   }

   //Waiting until processor 0 has finished.
   MPI_Barrier(comm);

   //processor 1
   if (rank == 1){

      cout << "I am processor "<< rank << " my vector c reads as follows:" << endl;

      for (int column = 0; column < 3; column++){
         cout << cvector[column] << " ";
      }
      cout << endl;

      cout << "I am processor "<< rank << " my row of A reads as follows:" << endl;
      for (int column = 0; column < 3; column++){
         cout << Arow[column] << " ";
      }
      cout << endl;

      cout << "I am processor "<< rank << " my matrix B reads as follows:" << endl;
      for (int row = 0; row < 3; row++){
         for (int column = 0; column < 3; column++){
	    cout << Bmatrix[row][column] << " ";
	 }
	 cout << endl;
      }

   }

   //Waiting until processor 1 has finished.
   MPI_Barrier(comm);

   //processor 2
   if (rank == 2){

      cout << "I am processor "<< rank << " my vector c reads as follows:" << endl;

      for (int column = 0; column < 3; column++){
         cout << cvector[column] << " ";
      }
      cout << endl;

      cout << "I am processor "<< rank << " my row of A reads as follows:" << endl;
      for (int column = 0; column <3; column++ ){
         cout << Arow[column] << " ";
      }
      cout << endl;

      cout << "I am processor "<< rank << " my matrix B reads as follows:" << endl;
      for (int row = 0; row < 3; row++){
         for (int column = 0; column < 3; column++){
	     cout << Bmatrix[row][column] << " ";
	     }
         cout << endl;
      }

   }


   //Waiting until processor 2 has finished before gathering the data.
   MPI_Barrier(comm);

   //Gathering data from each processor to construct the matrix D.
   MPI_Gather(&cvector, 3, MPI_FLOAT, Gathered_data, 3, MPI_FLOAT, 0, comm);

   //Waiting until the Gather is complete.
   MPI_Barrier(comm);

   //Constructing the matrix D on the root process.
   if (rank == 0){
      Dmatrix[0][0] = Gathered_data[0];
      Dmatrix[0][1] = Gathered_data[1];
      Dmatrix[0][2] = Gathered_data[2];
      Dmatrix[1][0] = Gathered_data[3];
      Dmatrix[1][1] = Gathered_data[4];
      Dmatrix[1][2] = Gathered_data[5];
      Dmatrix[2][0] = Gathered_data[6];
      Dmatrix[2][1] = Gathered_data[7];
      Dmatrix[2][2] = Gathered_data[8];


      //Printing gathered D from
      cout << "I am the root processor "<< rank << " and after gathering the data, D=AB reads as follows:" << endl;
      for (int row = 0; row < 3; row++){
         for (int column = 0; column < 3; column++){
	     cout << Dmatrix[row][column] << " ";
	     }
         cout << endl;
      }

   }

   //Ending MPI environment
   MPI_Finalize();

   return 0;
}
