#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>

// Parameters will be defined by Makefile compiler flags
#ifndef PARALLEL
#define PARALLEL 0 // Default to sequencial mode
#endif

// Default values for size, threads and repetitions
int uSize = 100;
int uThreads = 1;
int uReps = 100;

int fnbCheckMatrixResult(int **ppiMatrixA, int **ppiMatrixC, int uSize)
{
	for (int i = 0; i < uSize; i++)
		for (int j = 0; j < uSize; j++)
			if (ppiMatrixC[i][j] != ppiMatrixA[i][j])
				return 0;
	return 1;
}

void fnvAllocateMatrices(int uSize, int ***ppiMatrixA, int ***ppiMatrixB, int ***ppiMatrixC)
{
	*ppiMatrixA = malloc(uSize * sizeof(int *));
	*ppiMatrixB = malloc(uSize * sizeof(int *));
	*ppiMatrixC = malloc(uSize * sizeof(int *));

	for (int i = 0; i < uSize; i++)
	{
		(*ppiMatrixA)[i] = malloc(uSize * sizeof(int));
		(*ppiMatrixB)[i] = malloc(uSize * sizeof(int));
		(*ppiMatrixC)[i] = calloc(uSize, sizeof(int));
	}
}

void fnvFillMatrix(int uSize, int **ppiMatrix)
{
	for (int i = 0; i < uSize; i++)
		for (int j = 0; j < uSize; j++)
			ppiMatrix[i][j] = j + 1;
}

void fnvCreateIdentityMatrix(int uSize, int **ppiMatrix)
{
	for (int i = 0; i < uSize; i++)
		for (int j = 0; j < uSize; j++)
			ppiMatrix[i][j] = (i == j) ? 1 : 0;
}

void fnvZeroFillMatrix(int uSize, int **ppiMatrix)
{
	for (int i = 0; i < uSize; i++)
		for (int j = 0; j < uSize; j++)
			ppiMatrix[i][j] = 0;
}

void fnvMultiplyMatrices(int uSize, int **ppiMatrixA, int **ppiMatrixB, int **ppiMatrixC)
{
// Matrix multiplication: C = A × B
#if PARALLEL
#pragma omp parallel for collapse(2)
#endif
	for (int i = 0; i < uSize; i++)
		for (int j = 0; j < uSize; j++)
			for (int k = 0; k < uSize; k++)
				ppiMatrixC[i][j] += ppiMatrixA[i][k] * ppiMatrixB[k][j];
}

void fnvFreeMatrices(int uSize, int **ppiMatrixA, int **ppiMatrixB, int **ppiMatrixC)
{
	for (int i = 0; i < uSize; i++)
	{
		free(ppiMatrixA[i]);
		free(ppiMatrixB[i]);
		free(ppiMatrixC[i]);
	}
	free(ppiMatrixA);
	free(ppiMatrixB);
	free(ppiMatrixC);
}

int main(int argc, char *argv[])
{
	// Input parameters: matrix size, number of threads, number of repetitions
	if (argc >= 2)
		uSize = atoi(argv[1]);
	if (argc >= 3)
		uThreads = atoi(argv[1]);
	if (argc >= 4)
		uReps = atoi(argv[2]);

	double dStartTime, dEndTime;
	double dTotalTime = 0.0;
	double dMinTime = 99999999999;
	double dMaxTime = 0.0;

	int **ppiMatrixA, **ppiMatrixB, **ppiMatrixC;
#if PARALLEL
	omp_set_num_threads(uThreads);
#endif
#if PARALLEL
	printf("=== PARALLEL MODE (%d uThreads) ===\n", uThreads);
#else
	printf("=== SEQUENTIAL MODE ===\n");
#endif

	printf("Repetitions: %d\n", uReps);
	printf("Matrix size: %dx%d\n", uSize, uSize);

	fnvAllocateMatrices(uSize, &ppiMatrixA, &ppiMatrixB, &ppiMatrixC);
	fnvFillMatrix(uSize, ppiMatrixA);
	fnvCreateIdentityMatrix(uSize, ppiMatrixB);

	// Multiple executions for average calculation
	for (int i = 0; i < uReps; i++)
	{
		fnvZeroFillMatrix(uSize, ppiMatrixC);

		dStartTime = omp_get_wtime();
		fnvMultiplyMatrices(uSize, ppiMatrixA, ppiMatrixB, ppiMatrixC);
		dEndTime = omp_get_wtime();
		double dCurrentTime = (dEndTime - dStartTime) * 1000.0;

		dTotalTime += dCurrentTime;

		if (dCurrentTime < dMinTime)
			dMinTime = dCurrentTime;
		if (dCurrentTime > dMaxTime)
			dMaxTime = dCurrentTime;

		printf("Execution %2d: %f ms\n", i + 1, dCurrentTime);
	}

	// Result verification (only on last execution)
	printf("Verifying result...\n");
	if (fnbCheckMatrixResult(ppiMatrixA, ppiMatrixC, uSize))
	{
		printf("✓ Result CORRECT! A × I = A\n");
	}
	else
	{
		printf("✗ Result INCORRECT!\n");
	}
	// Statistics
	double dAverageTime = dTotalTime / uReps;
	printf("\n=== STATISTICS ===\n");
	printf("Average time: %f ms\n", dAverageTime);
	printf("Minimum time: %f ms\n", dMinTime);
	printf("Maximum time: %f ms\n", dMaxTime);
	printf("Variation: ±%f ms\n", (dMaxTime - dMinTime) / 2);
	printf("Speedup (min/max): %.2fx\n", dMaxTime / dMinTime);

	fnvFreeMatrices(uSize, ppiMatrixA, ppiMatrixB, ppiMatrixC);

	return 0;
}