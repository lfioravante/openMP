// Parallel matrix multiplication for a generic matrix of generic size and
// fixed input of value 1. The changeable variable is the number of rows
// which adjusts the number of columns too as rows = columns and
// produces a square matrix of size specified.

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <omp.h>

#ifndef PARALLEL
#define PARALLEL 0 // Default to sequencial mode
#endif

// define the amount of rows which essentially sets the
// size of the matrix as the rows = columns, therefore
// a square matrix is generated
#define ROWS 4
#define COLUMNS ROWS

// Initialising all three matrices
int MATRIX_A[ROWS][COLUMNS];
int MATRIX_B[ROWS][COLUMNS];
int MATRIX_C[ROWS][COLUMNS];

// Default values for threads and repetitions
int uThreads = 1;
int uReps = 100;

// Function to fill matrix A and matrix B with value 1
void Fill_Matrix(int mat[ROWS][COLUMNS])
{
#pragma omp parallel for collapse(2)
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
            mat[i][j] = 1;
    }
}

// Function written to print out resulting matrix
void printMatrix(int mat[ROWS][COLUMNS])
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
            printf("%d ", mat[i][j]);
        printf("\n");
    }
}

int main(int argc, char *argv[])
{
    // Ler argumentos da linha de comando
    if (argc >= 2)
	    uReps = atoi(argv[1]);
    if (argc >= 3)
		uThreads = atoi(argv[2]);
#if PARALLEL
    // Set number of threads
    omp_set_num_threads(uThreads);
#endif

    // Fill matrix A and matrix B using the above function
    Fill_Matrix(MATRIX_A);
    Fill_Matrix(MATRIX_B);

    double dTotalTime = 0.0;
    double dMinTime = 99999999; // Initialize with a very large value
    double dMaxTime = 0.0;
    double dAverageTime = 0.0;
    double start_time, end_time, current_time;

    // Multiple executions for average calculation
    for (int rep = 0; rep < uReps; rep++)
    {
        // Initialize result matrix to zeros
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLUMNS; j++)
                MATRIX_C[i][j] = 0;
        }

        start_time = omp_get_wtime();
        // multiplication of the 2 input matrices
#if PARALLEL
#pragma omp parallel for collapse(2)
#endif
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLUMNS; j++)
            {
                for (int k = 0; k < COLUMNS; k++)
                    MATRIX_C[i][j] += MATRIX_A[i][k] * MATRIX_B[k][j];
            }
        }
        end_time = omp_get_wtime();

        current_time = (end_time - start_time) * 1000.0;

        dTotalTime += current_time;
        if (current_time < dMinTime) dMinTime = current_time;
        if (current_time > dMaxTime) dMaxTime = current_time;
    }

    // Calculate average time
    dAverageTime = dTotalTime / uReps;

    printf("Matrix A is: \n");
    printMatrix(MATRIX_A);
    printf("Matrix B is: \n");
    printMatrix(MATRIX_B);
    printf("Resultant Matrix C, where Matrix A X Matrix B is: \n");
    printMatrix(MATRIX_C);

    printf("\n=== STATISTICS ===\n");
    printf("Average time: %f ms\n", dAverageTime);
    printf("Minimum time: %f ms\n", dMinTime);
    printf("Maximum time: %f ms\n", dMaxTime);
    printf("Variation: ±%f ms\n", (dMaxTime - dMinTime) / 2);
    printf("Speedup (min/max): %.2fx\n", dMaxTime / dMinTime);
    
    return 0;
}