/*
 * Program to perform matrix multiplication in parallel.
 * Author: Neo Zhou - zhouaea@bc.edu
 * Dylan Leddy - dylan.leddy@bc.edu
 */
 
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
 
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
 * Multiply two matrices A and B. Takes A, B, dim, the output file name, and p_num.
 */
void multiply(int n, int * A, int * B, char* file_name, int p_num) {
    // Open binary file to write product entries.
    FILE *fptr = fopen(file_name, "wb");
    if (fptr == NULL) {
        printf("File could not be opened\n");
    }
    
    int row_start;
    int row_end;
    
    // If p_num is -1, compute matrix in sequence.
    if (p_num < 0) {
        row_start = 0;
        row_end = n;
    // If p_num is greater than or equal to 0, then compute matrix in parallel.
    } else {
        row_start = p_num * n/4;
        row_end = (p_num + 1) * n/4;
    }
    
    // Calculate the whole matrix product or rows of the matrix product, depending on the p_num,
    // adding one entry at at time.
    for (int i = row_start; i < row_end; i++){
        for (int j = 0; j < n; j++) {
            int c_entry = 0;
            for (int k = 0; k < n; k++) {
                c_entry += A[i * n + k] * B[k * n + j];
            }
            // Write a single entry of the matrix product into a binary file.
            fwrite(&c_entry, sizeof(int), 1, fptr);
        }
    }
    fclose(fptr);
}

/*
* Reads data from the file into the serial matrix array.
*/
void read_entire_array(int n, int* matrix, char* file_name) {
    FILE* fptr = fopen(file_name, "rb");
    fread(matrix, sizeof(int), n * n, fptr);
    fclose(fptr);
}

/*
* Reassembles the matrix components into a single unit,
* using p_num to compute where in the array to write.
*/
void read_array(int n, int* matrix, char* file_name, int p_num) {
    FILE* fptr = fopen(file_name, "rb");
    fread(matrix + p_num * n * n/4, sizeof(int), n * n / 4, fptr);
    fclose(fptr);
}

/*
* Displays the passed matrix to the console
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
* Verifies that the parallel and serial matrices are
* the same.
*/
void verify(int* c, int* d, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (c[i * n + j] != d[i * n + j])
                printf("MATRICES ARE NOT THE SAME");
        }
    }
    printf("MATRICES ARE THE SAME");
}
 
int main(void) {
        // Set the width and height of the matrix as a power of 2.
    int e = 7;
    int n = pow(2,e);
    
    int * A = (int *) malloc(n * n * sizeof(int));
    int * B = (int *) malloc(n * n * sizeof(int));
    initialize_matrix(n, A);
    initialize_matrix(n, B);
    
    // Multiply randomly generated matrices A and B without parallelism, storing the product in a binary file. Measure how quickly the multiplication occurs.
    clock_t start;
    clock_t end;
    start = clock();
    multiply(n, A, B, "c_serial.bin", -1);
    end = clock();
      
    // Allocate space for the product matrix, read the contents of the serial binary file into it, and print it.
    int * serial = (int *) malloc(n * n * sizeof(int));
    read_entire_array(n, serial, "c_serial.bin");
    //print(n, serial);
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time elapsed after serial multiplication: %lf seconds \n\n", cpu_time_used);

    // Multiply A and B with parallelism, storing each partial product into seperate binary files, also measuring how quickly the multiplication occurs.
    // Keep track of the number of processes to index an array later.
    int p_num;
    char file_name[32];
    pid_t child_pids[3];
    pid_t pid;
    
    // Create three additional child processes to perform matrix multiplications on seperate rows.
    // The first child can be indexed through a p_num of 0, the second child can be indexed through a p_num of 1, and the third child can be indexed through a p_num of 2.
    start = clock();
    for (p_num = 0; p_num < 3; p_num++)
    {
        pid = fork();
        // If the current process is the child process, calculate a subset of rows of the matrix and then exit. Only the parent should be forking.
        if (pid == 0) {
            sprintf(file_name, "c_parallel%i.bin", p_num);
            multiply(n, A, B, file_name, p_num);
            exit(0);
        // If the current process is the parent process, store the process id of each child spawned.
        } else {
            child_pids[p_num] = pid;
        }
    }
    
    // Have the parent process calculate a subset of the rows of the product matrix as well.
    sprintf(file_name, "c_parallel%i.bin", p_num);
    multiply(n, A, B, file_name, p_num);
    
    // Combine the results of the matrices after all processes have finished.
    for (p_num = 0; p_num < 3; p_num++) {
        pid = child_pids[p_num];
        
        int status;
        waitpid(pid, &status, 0);
    }
    end = clock();

        
    // Allocate space for the product matrix, read the contents of the seperate serial binary files into it, and print it.
    int* parallel = (int*)malloc(n * n * sizeof(int));
    for (p_num = 0; p_num < 4; ++p_num) {
        sprintf(file_name, "c_parallel%d.bin", p_num);
        read_array(n, parallel, file_name, p_num);
    }
    //print(n, parallel);
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time elapsed after parallel multiplication: %lf seconds \n\n", cpu_time_used);
    
    // Ensure that the serial and paralell multiplication methods yield the same result.
    verify(serial, parallel, n);

    free(A);
    free(B);
    free(serial);
    free(parallel);
}


// Multiply A and B with parallelism, storing each partial product into seperate binary files, also measuring how quickly the multiplication occurs.
   // Keep track of the number of processes to index an array later.
   int t_num;
   pthread_t tids[3];
   pid_t pid;
   
   // Create three additional child processes to perform matrix multiplications on seperate rows.
   // The first child can be indexed through a t_num of 0, the second child can be indexed through a t_num of 1, and the third child can be indexed through a t_num of 2.
   start = clock();
   for (t_num = 0; t_num < 3; t_num++)
   {
       // If the current process is the child process, calculate a subset of rows of the matrix and then exit. Only the parent should be forking.
       if (pid == 0) {
           multiply(n, A, B, D, t_num);
           exit(0);
       // If the current process is the parent process, store the process id of each child spawned.
       } else {
           tids[t_num] = pid;
       }
   }
   
   // Have the parent process calculate a subset of the rows of the product matrix as well.
   sprintf(file_name, "c_parallel%i.bin", t_num);
   multiply(n, A, B, file_name, t_num);
   
   // Combine the results of the matrices after all processes have finished.
   for (t_num = 0; t_num < 3; t_num++) {
       pid = tids[t_num];
       
       int status;
       waitpid(pid, &status, 0);
   }
   end = clock();

       
   // Allocate space for the product matrix, read the contents of the seperate serial binary files into it, and print it.
   int* parallel = (int*)malloc(n * n * sizeof(int));
   for (p_num = 0; p_num < 4; ++p_num) {
       sprintf(file_name, "c_parallel%d.bin", p_num);
       read_array(n, parallel, file_name, p_num);
   }
   //print(n, parallel);
   cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
   printf("Time elapsed after parallel multiplication: %lf seconds \n\n", cpu_time_used);
   
   // Ensure that the serial and paralell multiplication methods yield the same result.
   verify(serial, parallel, n);

   free(A);
   free(B);
   free(serial);
   free(parallel);
}
