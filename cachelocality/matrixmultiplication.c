/*
 * Program to multiply square matrices efficiently.
 * Author: Neo Zhou - zhouaea@bc.edu
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * Initialize a matrix with pseudo-random numbers from 0-9.
 */
void initialize_matrix(int n, int * matrix) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i * n + j] = rand() % 10;
        }
    }
}

/*
 * Print the entries of a matrix in a visually organized manner.
 */
void print_matrix(int n, int * matrix) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%i ", matrix[i * n + j]);
        }
        printf("\n");
    }
    printf("\n");
}

/*
 * Multiply two matrices A and B without transposition.
 */
void multiply_standard(int n, int * A, int * B, int * C) {
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++) {
            C[i * n + j] = 0;
            for (int k = 0; k < n; k++) {
                C[i * n + j] += A[i * n + k] * B[k * n + j];
            }
        }
    }
}

/*
 * Transpose in place by iterating over only the top right diagonal half of the matrix and
 * swapping elements.
 */
void transpose(int n, int * matrix) {
    int temp;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            temp = matrix[i * n + j];
            matrix[i * n + j] = matrix[j * n + i];
            matrix[j * n + i] = temp;
        }
    }
}

/*
 * Multiply two matrices A and B, first transposing B to get better spacial locality.
 */
void multiply_transpose(int n, int * A, int * B, int * D) {
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++) {
            D[i * n + j] = 0;
            for (int k = 0; k < n; k++) {
                D[i * n + j] += A[i * n + k] * B[j * n + k];
            }
        }
    }
}

/*
 * Check the corresponding entries of two matrices to see if they have the same values.
 */
void verify(int n, int * C, int * D) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (C[i * n + j] != D[i * n + j]) {
                printf("RESULTS ARE NOT THE SAME\n");
                return;
            }
        }
    }
    printf("RESULTS ARE THE SAME\n");
}

/*
 * Randomly generate n x n matrices, multiply them together using two different methods, and measure
 * performance.
 */
void run(int n) {
    // Declare and initialize four matrices in the heap.
    int * A = (int *) malloc(n * n * sizeof(int));
    int * B = (int *) malloc(n * n * sizeof(int));
    int * C = (int *) malloc(n * n * sizeof(int));
    int * D = (int *) malloc(n * n * sizeof(int));
    initialize_matrix(n, A);
    initialize_matrix(n, B);
    
    printf("A:\n");
    print_matrix(n, A);
    printf("B:\n");
    print_matrix(n, B);
    
    clock_t start;
    clock_t end;

    // Multiply A and B using the standard method and measure its performance.
    start = clock();
    multiply_standard(n, A, B, C);
    end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("A x B using standard multiplication:\n");
    print_matrix(n, C);
    printf("Time elapsed after standard multiplication: %lf seconds \n\n", cpu_time_used);

    // Multiply A and B using the transposed method and measure its performance.
    start = clock();
    transpose(n, B);
    multiply_transpose(n, A, B, D);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("A x B using transposed multiplication:\n");
    print_matrix(n, D);
    printf("Time elapsed after tranposed multiplication: %lf seconds\n\n", cpu_time_used);
    
    // Ensure matrices have the same entires.
    verify(n, C, D);

    free(A);
    free(B);
    free(C);
    free(D);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Format: ./matrixmultiplication base_2_exponent\n");
        return 1;
    }
    
    // Ensure the maximum value of n is 1024 and the minimum value of n is 1.
    int power = atoi(argv[1]);
    if (power < 0 || power > 10) {
        printf("Please input an exponent between 0 and 10");
        return 1;
    }
    
    int n = pow(2, power);
    
    printf("n = %i\n", n);
    run(n);

    return 0;
}
