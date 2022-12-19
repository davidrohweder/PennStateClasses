/**
 * @file cannon.cpp
 * @author David Rohweder, djr6005@psu.edu
 * @brief Cannon's MMM, using MPI on ACI
 * @version 0.1
 * @date 2022-04-27
 *
 * @copyright Copyright (c) 2022
 *
 */


 // ***** Region Start: Includes	


#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <sys/time.h>


// ***** Region End: Includes


// ***** Region Start: Project Setup


#define N 100

void get_walltime(double* wcTime);
void printMatrix(long*, int);
void serial_MMM(int, long*, long*, long*);
void cannon_MMM_MPI(long*, long*, long*, int, MPI_Comm);
int validate_MPI_Serial(long*, long*);
void prepareMatricies(long*, long*, long*, long*);


// ***** Region End: Project Setup	


// ***** Region Start: Cannon's MMM Using MPI


/*
	MPI Cannon's MMM Algorithm --- * Testing A*Id=C ::: C should in theroy be 2x A since the identity of diagonal 2 will double A
*/
int  main(int argc, char* argv[]) {

	int testSerialFlag = 0;


	// ***** Region Start: MPI Exe.


	MPI_Init(&argc, &argv); // start communication between nodes

	double mpi_wcs, mpi_wce, mpi_duration;
	mpi_wcs = MPI_Wtime();

	int P; // &argc *= mpirun -np P
	MPI_Comm_size(MPI_COMM_WORLD, &P); // create P cart
	int sqP = sqrt(P);

	int rank; // node number where code is being executed
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);


	// ***** Sub-Region Start: Env. Setup

	int subSize = N / sqP;

	long* sub_matrix_a = new long[P];
	long* sub_matrix_b = new long[P];
	long* sub_matrix_c = new long[P];
	long* matrix_a;
	long* matrix_b;
	long* matrix_c;
	long* serial_matrix_c;

	for (int i = 0; i < sqP; i++) {
		for (int j = 0; j < sqP; j++) {
			sub_matrix_c[i * sqP + j] = 0;
		}
	}

	MPI_Datatype subarr, data_type;

	int array_size[2] = { P, P };
	int subarray_sizes[2] = { sqP, sqP };
	int array_start[2] = { 0, 0 };

	MPI_Type_create_subarray(2, array_size, subarray_sizes, array_start, MPI_ORDER_C, MPI_LONG, &subarr);
	MPI_Type_create_resized(subarr, 0, sqrt(P) * sizeof(long), &data_type);
	MPI_Type_commit(&data_type);
	// src - https://www.rookiehpc.com/mpi/docs/mpi_order_c.php

	int counts_send[P];
	int displacements[P];

	if (rank == 0) {

		matrix_a = new long[N * N];
		matrix_b = new long[N * N];
		matrix_c = new long[N * N];
		serial_matrix_c = new long[N * N];

		prepareMatricies(matrix_a, matrix_b, matrix_c, serial_matrix_c);

		// print A and B matrix to visualize graph data
		if (testSerialFlag == 1 && rank == 0) {
			printf("Matrix A: Rank %d \n", rank);
			printMatrix(matrix_a, N);
			printf("\n");
			printf("Matrix B: rank %d \n", rank);
			printMatrix(matrix_b, N);
			printf("\n");
		}

		// calculate offsets for submatricies
		for (int i = 0; i < P; i++) {
			counts_send[i] = 1;
		}

		int displacement = 0;

		for (int i = 0; i < sqP; i++) {
			for (int j = 0; j < sqP; j++) {
				displacements[i * sqP + j] = displacement;
				displacement += 1;
			}
			displacement += (P / sqP - 1) * sqP;
		}
	}

	// scatter sub matricies to all other nodes
	MPI_Scatterv(matrix_a, counts_send, displacements, data_type, sub_matrix_a, P, MPI_LONG, 0, MPI_COMM_WORLD);
	MPI_Scatterv(matrix_b, counts_send, displacements, data_type, sub_matrix_b, P, MPI_LONG, 0, MPI_COMM_WORLD);
	// src - https://www.rookiehpc.com/mpi/docs/mpi_scatterv.php#:~:text=Definition,communicator%20must%20invoke%20this%20routine

	if (testSerialFlag == 1) {
		printMatrix(sub_matrix_a, sqP);
		printMatrix(sub_matrix_b, sqP);
	}


	// ***** Sub-Region Start: Env. Setup


	// ***** Sub-Region Start: Run Cannon's Algorithm


	MPI_Barrier(MPI_COMM_WORLD); // barrier to make sure all sub matricies are initalizes -- > then begin on all nodes

	cannon_MMM_MPI(sub_matrix_a, sub_matrix_b, sub_matrix_c, P, MPI_COMM_WORLD); // broken into sub per submission requirment
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Gatherv(sub_matrix_c, P, MPI_LONG, matrix_c, counts_send, displacements, data_type, 0, MPI_COMM_WORLD); // gather partial sub C from nodes into C

	MPI_Barrier(MPI_COMM_WORLD); // ensure all nodes are synced before returning to rank 0 node

	mpi_wce = MPI_Wtime();
	mpi_duration = mpi_wce - mpi_wcs;


	// ***** Sub-Region End: Run Cannon's Algorithm


	// ***** Sub-Region Start: Validate MPI C vs Serial C


	if (testSerialFlag == 1 && rank == 0) {
		double s_wcs, s_wce, s_duration;
		get_walltime(&s_wcs);
		serial_MMM(N, matrix_a, matrix_b, serial_matrix_c);

		get_walltime(&s_wce);
		s_duration = s_wce - s_wcs;

		int correctOutput = validate_MPI_Serial(matrix_c, serial_matrix_c);
		if (correctOutput == 1) {
			printf("\n Wall times: MPI: %f, Serial: %f\n", mpi_duration, s_duration);
		}
	}


	// ***** Sub-Region End: Validate MPI C vs Serial C


	// ***** Sub-Region Start: Env. Cleanup


	if (rank == 0) {
		delete[] matrix_a;
		delete[] matrix_b;
		delete[] matrix_c;
		delete[] serial_matrix_c;
	}

	delete[] sub_matrix_a;
	delete[] sub_matrix_b;
	delete[] sub_matrix_c;


	// ***** Sub-Region End: Env. Cleanup	    


	MPI_Finalize(); // - end communication between nodes and have rank 0 return to coninue processing -


	// ***** Region End: MPI Exe.


	return(0);

}


/*
	Serial MMM - for both MPI submatricies and serial comparison
*/
void serial_MMM(int n, long* matrix_a, long* matrix_b, long* matrix_c) {

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			for (int k = 0; k < n; k++) {
				matrix_c[i * n + j] += matrix_a[i * n + k] * matrix_b[k * n + j];
			}
		}
	}
}


/*
	MPI Cannon's - Cartesian shifts and local multiplies
*/
void cannon_MMM_MPI(long* sub_matrix_a, long* sub_matrix_b, long* sub_matrix_c, int P, MPI_Comm old_comm) {


	// ***** Region Start: Cart Setup


	int pAxisLength = sqrt(P);

	int ndims = 2; // 2d cart grid
	int dims[2] = { pAxisLength, pAxisLength }; // sqrt(P) * sqrt(P) 2d cart grid
	int periodical[2] = { 1, 1 }; // circular shifts in both directions
	int reorder = 1; // ranks stay same
	int coords[2];
	int remain_dims[2] = { 0,1 };
	int cart_rank;
	MPI_Comm cart_comm;
	MPI_Cart_create(old_comm, ndims, dims, periodical, reorder, &cart_comm);
	MPI_Comm_rank(cart_comm, &cart_rank);
	MPI_Cart_coords(cart_comm, cart_rank, ndims, coords);

	MPI_Comm row_comm;
	MPI_Comm col_comm;

	MPI_Cart_sub(cart_comm, remain_dims, &row_comm);

	remain_dims[0] = 1;
	remain_dims[1] = 0;
	MPI_Cart_sub(cart_comm, remain_dims, &col_comm);
	// src - https://www.rookiehpc.com/mpi/docs/mpi_cart_sub.php


	// ***** Region End: Cart Setup


	// ***** Region Start: 2d grid init -- linedancing

	long* old_matrix_a = sub_matrix_a;
	long* old_matrix_b = sub_matrix_b;

	int source, destination;
	MPI_Status status;
	int up, down, left, right;

	// Get neighbors
	MPI_Cart_shift(cart_comm, 1, -1, &right, &left);
	MPI_Cart_shift(cart_comm, 0, -1, &down, &up);

	// Skew phase
	MPI_Cart_shift(cart_comm, 1, -coords[0], &source, &destination);
	MPI_Sendrecv_replace(sub_matrix_a, P, MPI_LONG, destination, 1, source, 1, cart_comm, &status);
	MPI_Cart_shift(cart_comm, 0, -coords[1], &source, &destination);
	MPI_Sendrecv_replace(sub_matrix_b, pAxisLength * pAxisLength, MPI_LONG, destination, 1, source, 1, cart_comm, &status);

	// multiply and linedance ;) the matrix A and matrix B
	for (int i = 0; i < pAxisLength; i++) {
		serial_MMM(pAxisLength, sub_matrix_a, sub_matrix_b, sub_matrix_c);
		MPI_Sendrecv_replace(sub_matrix_a, P, MPI_LONG, left, 1, right, 1, cart_comm, &status);
		MPI_Sendrecv_replace(sub_matrix_b, P, MPI_LONG, up, 1, down, 1, cart_comm, &status);
	}

	// Restore A & B Blocks 
	sub_matrix_a = old_matrix_a;
	sub_matrix_b = old_matrix_b;


	// ***** Region Start: Env. cleanup


	MPI_Comm_free(&cart_comm);


	// ***** Region Start: Env. cleanup


}


/*
	compare serial vs mpi cannon matricies results of matrix_c vs serial_matrix_c :: NOTE: runs only if
*/
int validate_MPI_Serial(long* matrix_c, long* serial_matrix_c) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (matrix_c[i * N + j] != serial_matrix_c[i * N + j]) {
				printf("Error, matrix values different at: [%i, %i]... MPI: %d, Serial %d\n", i, j, matrix_c[i * N + j], serial_matrix_c[i * N + j]);
				printf("Matrix Serial C: \n");
				printMatrix(serial_matrix_c, N);
				printf("\n");
				printf("Matrix MPI C: \n");
				printMatrix(matrix_c, N);
				printf("\n");
				return(0);
			}
		}
	}
	return(1);
}


/*
	matrix setup:: NOTE: B ===-> 2*B_Id
*/
void prepareMatricies(long* matrix_a, long* matrix_b, long* matrix_c, long* serial_matrix_c) {

	for (long i = 0; i < N; i++) {

		for (long j = 0; j < N; j++) {
			matrix_a[i * N + j] = j;

			// double identity matrix
			if (i == j) {
				matrix_b[i * N + j] = 2;
			}
			else {
				matrix_b[i * N + j] = 0;
			}

			matrix_c[i * N + j] = 0;
			serial_matrix_c[i * N + j] = 0;
		}
	}
}


/*
	print matricies to validate results visually:: prints big matrix and sub matrix (Hence, int n)
*/
void printMatrix(long* matrix, int n) {

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			printf("%ld\t", matrix[i * n + j]);
		}
		printf("\n");
	}

}


/*
	Canned walltime code - src - William Seisler
*/
void get_walltime(double* wcTime)
{
	struct timeval tp;
	gettimeofday(&tp, NULL);
	*wcTime = (double)(tp.tv_sec + tp.tv_usec / 1000000.0);
}