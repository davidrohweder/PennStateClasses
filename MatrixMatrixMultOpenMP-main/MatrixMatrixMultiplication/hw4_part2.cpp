// Author: David Rohweder

#include <hw4_part2.hpp>
#include <omp.h>
#include <stdio.h>


// complex algorithm for evaluation
void matrix_mult(double *A, double *B, double *C, int N)
{
	int block = 10;
	int P = 1;
	omp_set_num_threads(P);	


#pragma omp parallel for shared(block) schedule(auto) collapse(3)
    for (int i = 0; i < N; i += block) {
        
        for (int j = 0; j < N; j += block) {
            for (int k = 0; k < N; k += block) { 
                for (int ii = i; ii < i + block; ii++) {
                    for (int jj = j; jj < j + block; jj++) {
                        double sum = 0.0;
                        
                        for (int kk = k; kk < k + block; kk++) {
                            sum += A[ii * N + kk] * B[kk * N + jj];
                        }
                        C[ii * N + jj] += sum;
                    }
                }
            }
        }
    }
	
}
