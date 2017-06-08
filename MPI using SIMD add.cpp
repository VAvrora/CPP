/*
Programm adds 2 arrays in parallel using two processes and SIMD add
how to compile: https://blogs.technet.microsoft.com/windowshpc/2015/02/02/how-to-compile-and-run-a-simple-ms-mpi-program/
*/
#include "mpi.h"
#include "stdio.h"
#include "string.h"
#include <vector>
#include <iostream>
#include "stdlib.h"

#define SIZE 1024
#define size SIZE/2

int main(int argc, char **argv)
{
	int message;
	int myrank;

	MPI_Status status;
	MPI_Comm comm;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	
	float input1[SIZE];
	float input2[SIZE];
	

	__m128 stam1, stam2;


	if (myrank == 0)    /* code for process 0*/
	{
	
		float output1[SIZE];
		float output0[SIZE];


		for (int i = 0; i < SIZE; i++)
		{
			input1[i] = i;
			input2[i] = i * 2;
		
		}
		
		//send first part of input1 to rank1
		MPI_Send(input1, size, MPI_FLOAT, 1, 0, MPI_COMM_WORLD);
		//send first part of input2 to rank1
		MPI_Send(input2, size, MPI_FLOAT, 1, 0, MPI_COMM_WORLD);

		//fastadd usimg SIMD
		for (int i = size; i < SIZE; i += 4)
		{
			stam1 = _mm_loadu_ps(&(input1[i]));
			stam2 = _mm_loadu_ps(&(input2[i]));
			stam1 = _mm_add_ps(stam1, stam2);
			_mm_storeu_ps(&(output0[i]), stam1);
		}
		
		//receive second part of input2
		MPI_Recv(output1, size, MPI_FLOAT, 1, 0, MPI_COMM_WORLD, &status);
		
		

		for (int i = 0; i < size; i+=4){
			stam1 = _mm_loadu_ps(&(output1[i]));
			_mm_storeu_ps(&(output0[i]), stam1);
		}

		for (int i = 0; i < SIZE; i++)
			printf(" output=:\t%.2f\n", output0[i]);
	}

	else   /* code for process one */
	{


		float output1[SIZE];

		//receive first part of input1
		MPI_Recv(input1, size, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &status);
		//receive first part of input1
		MPI_Recv(input2, size, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &status);

		//fastadd usimg SIMD
		for (int i = 0; i < size; i += 4)
		{
			stam1 = _mm_loadu_ps(&(input1[i]));
			stam2 = _mm_loadu_ps(&(input2[i]));
			stam1 = _mm_add_ps(stam1, stam2);
			_mm_storeu_ps(&(output1[i]), stam1);
		}

		//send first part of input2 to rank1*/
		MPI_Send(output1, size, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
				
	}
	MPI_Finalize();
}
