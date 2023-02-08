/*
 * Program to perform matrix multiplication in parallel using POSIX threads.
 * Author: Neo Zhou - zhouaea@bc.edu
 * Dylan Leddy - dylan.leddy@bc.edu
 */
 
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct multiply_parameters {
    int n;
    int * A;
    int * B;
    int * C;
    int t_num;
} multiply_parameters;

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
 * Multiply two matrices A and B. Takes a structure, multiply_parameters_arg, containing the
 * variables n, A, B, C, and t_num.
  */
void * multiply(void* multiply_parameters_arg) {
    multiply_parameters * thread_parameters = (multiply_parameters *) multiply_parameters_arg;
    int row_start;
    int row_end;
    
    int n = thread_parameters->n;
    int * A = thread_parameters->A;
    int * B = thread_parameters->B;
    int * C = thread_parameters->C;
    int t_num = thread_parameters->t_num;
    
    // If t_num is -1, compute matrix in sequence.
    if (t_num < 0) {
        row_start = 0;
        row_end = n;
    // If t_num is greater than or equal to 0, then compute matrix in parallel.
    } else {
        row_start = t_num * n / 4;
        row_end = (t_num + 1) * n / 4;
    }
    
    // Calculate the whole matrix product or rows of the matrix product, depending on the t_num,
    // adding one entry at at time.
    for (int i = row_start; i < row_end; i++) {
        for (int j = 0; j < n; j++) {
            int c_entry = 0;
            for (int k = 0; k < n; k++) {
                c_entry += A[i * n + k] * B[k * n + j];
            }
            // Write a set number of rows of the matrix product into the output matrix.
            C[i * n + j] = c_entry;
        }
    }
    
    return NULL;
}

/*
* Display the entries of a matrix to the console.
*/
void print(int n, int* matrix) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", matrix[i * n + j]);
        }
        printf("\n");
    }
}

/*
* Verify that the parallel and serial matrices are
* the same.
*/
void verify(int* C, int* D, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (C[i * n + j] != D[i * n + j])
                printf("MATRICES ARE NOT THE SAME");
        }
    }
    printf("MATRICES ARE THE SAME");
}

int main(void) {
    // Set the width and height of the matrix as a power of 2.
    int e = 4;
    int n = pow(2,e);
    
    // Initialize A and B as n x n matrices with random values and allocate space for two product
    // matrices: one will hold the results of serial matrix multiplication and the other parallel.
    int * A = (int *) malloc(n * n * sizeof(int));
    int * B = (int *) malloc(n * n * sizeof(int));
    int * serial = (int *) malloc(n * n * sizeof(int));
    int * parallel = (int *) malloc(n * n * sizeof(int));
    initialize_matrix(n, A);
    initialize_matrix(n, B);
    
    // Multiply randomly generated matrices A and B without parallelism, storing the product in
    // another matrix. Measure how quickly the multiplication occurs.
    clock_t start;
    clock_t end;
    start = clock();
    multiply_parameters serial_multiplication_parameters = {n, A, B, serial, -1};
    multiply(&serial_multiplication_parameters);
    end = clock();

    //print(n, serial);
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time elapsed after serial multiplication: %lf seconds \n\n", cpu_time_used);

    // Multiply A and B with parallelism, storing each partial product into a subset of an output
    // matrix, also measuring how quickly the multiplication occurs. Keep track of the thread ids so
    // we can wait for them to end later.
    int t_num;
    pthread_t tids[3];
    
    // Keep track of each thread's function parameters via an array of structs, since we have to
    // pass in a struct into a thread to pass in multiple arguments.
    multiply_parameters thread_parameters[3];

    // Create three additional threads to perform matrix multiplications on seperate rows.
    // The first thread can be indexed through a t_num of 0, the second thread can be indexed
    // through a t_num of 1, and the third thread can be indexed through a t_num of 2.
    start = clock();
    
    // Initialize a thread and its parameters in a struct to do a portion of the matrix multiplicatoin.
    for (t_num = 0; t_num < 3; t_num++) {
        // Initialize the parameters that will be needed for each process.
        thread_parameters[t_num] = (multiply_parameters) {n, A, B, parallel, t_num};
        // Create a new thread that will run the multiply function with its own set of parameters.
        pthread_create(&tids[t_num], NULL, multiply, &thread_parameters[t_num]);
    }
   
    // Have the parent process calculate a subset of the rows of the product matrix as well.
    multiply_parameters parent_parameters = {n, A, B, parallel, t_num};
    multiply(&parent_parameters);
   
    // Wait until all processes have finished before looking at the output matrix.
    for (t_num = 0; t_num < 3; t_num++) {
        pthread_join(tids[t_num], NULL);
    }
    end = clock();

    //print(n, parallel);
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time elapsed after parallel multiplication: %lf seconds \n\n", cpu_time_used);
   
    // Ensure that the serial and parallel multiplication methods yield the same result.
    verify(serial, parallel, n);

    free(A);
    free(B);
    free(serial);
    free(parallel);
}
