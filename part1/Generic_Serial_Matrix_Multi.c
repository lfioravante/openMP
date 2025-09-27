#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>


// Parameters will be defined by Makefile compiler flags
#ifndef PARALLEL
#define PARALLEL 0 // Default to sequencial mode
#endif

#ifndef REPETITIONS
#define REPETITIONS 1 // Default repetitions
#endif

int fnbCheckMatrixResult(int **ppiMatrixA, int **ppiMatrixC, int iSize)
{
	for (int i = 0; i < iSize; i++)
		for (int j = 0; j < iSize; j++)
			if (ppiMatrixC[i][j] != ppiMatrixA[i][j])
				return 0;
	return 1;
}

void fnvAllocateMatrices(int iSize, int ***ppiMatrixA, int ***ppiMatrixB, int ***ppiMatrixC)
{
	*ppiMatrixA = malloc(iSize * sizeof(int *));
	*ppiMatrixB = malloc(iSize * sizeof(int *));
	*ppiMatrixC = malloc(iSize * sizeof(int *));

	for (int i = 0; i < iSize; i++)
	{
		(*ppiMatrixA)[i] = malloc(iSize * sizeof(int));
		(*ppiMatrixB)[i] = malloc(iSize * sizeof(int));
		(*ppiMatrixC)[i] = calloc(iSize, sizeof(int));
	}
}

void fnvFillMatrix(int iSize, int **ppiMatrix)
{
	for (int i = 0; i < iSize; i++)
		for (int j = 0; j < iSize; j++)
			ppiMatrix[i][j] = j + 1;
}

void fnvCreateIdentityMatrix(int iSize, int **ppiMatrix)
{
	for (int i = 0; i < iSize; i++)
		for (int j = 0; j < iSize; j++)
			ppiMatrix[i][j] = (i == j) ? 1 : 0;
}

void fnvZeroFillMatrix(int iSize, int **ppiMatrix)
{
	for (int i = 0; i < iSize; i++)
		for (int j = 0; j < iSize; j++)
			ppiMatrix[i][j] = 0;
}

void fnvMultiplyMatrices(int iSize, int **ppiMatrixA, int **ppiMatrixB, int **ppiMatrixC)
{
// Matrix multiplication: C = A × B
#pragma omp parallel for collapse(2) if (PARALLEL)
	for (int i = 0; i < iSize; i++)
		for (int j = 0; j < iSize; j++)
			for (int k = 0; k < iSize; k++)
				ppiMatrixC[i][j] += ppiMatrixA[i][k] * ppiMatrixB[k][j];
}

void fnvFreeMatrices(int iSize, int **ppiMatrixA, int **ppiMatrixB, int **ppiMatrixC)
{
	for (int i = 0; i < iSize; i++)
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
	if (argc != 3)
	{
		printf("Usage: %s <iMatrixSize> <iThreadCount>\n", argv[0]);
		printf("Compile-time settings: PARALLEL=%d, REPETITIONS=%d\n", PARALLEL, REPETITIONS);
		return 1;
	}
#if !PARALLEL

#endif
	int iSize = atoi(argv[1]);
#if PARALLEL
	int iThreads = atoi(argv[2]);
#else
		struct timeval t1, t2;
#endif
	double dStartTime, dEndTime;
	double dTotalTime = 0.0;
	double dMinTime = 99999999999;
	double dMaxTime = 0.0;

	int **ppiMatrixA, **ppiMatrixB, **ppiMatrixC;
#if PARALLEL
	omp_set_num_threads(iThreads);
#endif
#if PARALLEL
	printf("=== PARALLEL MODE (%d iThreads) ===\n", iThreads);
#else
	printf("=== SEQUENTIAL MODE ===\n");
#endif

	printf("Repetitions: %d\n", REPETITIONS);
	printf("Matrix size: %dx%d\n", iSize, iSize);

	fnvAllocateMatrices(iSize, &ppiMatrixA, &ppiMatrixB, &ppiMatrixC);
	fnvFillMatrix(iSize, ppiMatrixA);
	fnvCreateIdentityMatrix(iSize, ppiMatrixB);

	// Multiple executions for average calculation
	for (int i = 0; i < REPETITIONS; i++)
	{
		fnvZeroFillMatrix(iSize, ppiMatrixC);

#if PARALLEL
		dStartTime = omp_get_wtime();
#else
		gettimeofday(&t1, NULL); 
#endif

		fnvMultiplyMatrices(iSize, ppiMatrixA, ppiMatrixB, ppiMatrixC);

#if PARALLEL
		dEndTime = omp_get_wtime();
		double dCurrentTime = (dEndTime - dStartTime) * 1000.0;
#else
		gettimeofday(&t2, NULL);
		double dCurrentTime = (t2.tv_sec - t1.tv_sec) * 1000.0 + (t2.tv_usec - t1.tv_usec) / 1000.0;
#endif

		dTotalTime += dCurrentTime;

		if (dCurrentTime < dMinTime)
			dMinTime = dCurrentTime;
		if (dCurrentTime > dMaxTime)
			dMaxTime = dCurrentTime;

		printf("Execution %2d: %f ms\n", i + 1, dCurrentTime);
	}
#if PARALLEL
	// Result verification (only on last execution)
	printf("Verifying result...\n");
	if (fnbCheckMatrixResult(ppiMatrixA, ppiMatrixC, iSize))
	{
		printf("✓ Result CORRECT! A × I = A\n");
	}
	else
	{
		printf("✗ Result INCORRECT!\n");
	}
#endif
	// Statistics
	double dAverageTime = dTotalTime / REPETITIONS;
	printf("\n=== STATISTICS ===\n");
	printf("Average time: %f ms\n", dAverageTime);
	printf("Minimum time: %f ms\n", dMinTime);
	printf("Maximum time: %f ms\n", dMaxTime);
	printf("Variation: ±%f ms\n", (dMaxTime - dMinTime) / 2);
	printf("Speedup (min/max): %.2fx\n", dMaxTime / dMinTime);

	fnvFreeMatrices(iSize, ppiMatrixA, ppiMatrixB, ppiMatrixC);

	return 0;
}