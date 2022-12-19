// Author: David Rohweder
// CS 450
// MAR 9 2022

#include <hw3_part2.hpp>
#include <thread>


// Global Structure Defines


typedef struct threadInformation {
	int start;
	int stop;
	double *ptrA;
	double *ptrB;
	double *ptrC;
	int block;
	int N;
} threadInformation;


// Start Code Region


/*
  threaded function without optimizations
*/
void threaded_mmm(threadInformation& info) {
	
	for (int row = info.start; row < info.stop; row++) {
		
		for (int j = 0; j < info.N; j++) {
			info.ptrC[row * info.N + j] = 0;

			for (int k = 0; k < info.N; k++) {
				info.ptrC[row * info.N + j] += info.ptrA[row * info.N + k] * info.ptrB[k * info.N + j];
			}
		}
	}
}


/*
  threaded function with optimizations
*/
void threaded_opt_mmm(threadInformation& info) {

	for (int row = info.start; row < info.stop; row++) {

		for (int block = 0; block < info.N; block += info.block) {

			for (int ii = 0; ii < info.N; ii += info.block) {
        
				for (int jj = 0; jj < info.block; jj++) {
          
          			for (int k  = 0; k  < info.block; k ++) {
			  			info.ptrC[row * info.N + block + k] += info.ptrA[row * info.N + ii + jj] * info.ptrB[ii * info.N + jj * info.N + block + k];
		  			}
				}
	  		}
		}
  	}	  
}

// complex algorithm for evaluation
void matrix_mult(double *A, double *B, double *C, int N)
{
	int i = 0;	

	// num threads, version on canvas didnt have param thru func so i made local var :: also makerfile did not have -pthread flag
	int P = 20;

	// Define threads 
	std::thread threads[P];
	threadInformation* threadInfo = new threadInformation[P];

	// Give each thread local copies of information
	for (i = 0; i < P; i++) {
		threadInfo[i].start = i * (N/P); // calculates start rows
		threadInfo[i].stop = (i + 1) * (N/P); // calculates end row
		threadInfo[i].ptrA = A;
		threadInfo[i].ptrB = B;
		threadInfo[i].ptrC = C;
		threadInfo[i].block = 500;
		threadInfo[i].N = N;
	}

	// Fire off threads
	for (i = 0; i < P; i++) {
		threads[i] = std::thread(threaded_opt_mmm, std::ref(threadInfo[i]));
	}

	// Join threads
	for (i = 0; i < P; i++) {
		threads[i].join();
	}
}

