//Sequential matrix multiplication for a generic matrix of generic size and 
//fixed input of value 1. The changeable variable is the number of rows
//which adjusts the number of columns too as rows = columns and 
//produces a square matrix of sise specified. 

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/time.h>

//define the amount of rows which essentially sets the 
//size of the matrix as the rows = columnns, therefore 
//a square matrix is generated 
#define ROWS 5
#define COLUMNS ROWS

//Initialising all three matrices 
int MATRIX_A[ROWS][COLUMNS];
int MATRIX_B[ROWS][COLUMNS];
int MATRIX_C[ROWS][COLUMNS];

//Function to fill matrix A and matrix B with value 1
void Fill_Matrix(int mat[ROWS][COLUMNS]) { 
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++)
			mat[i][j] = 1;
	}
}

//Function written to print out resulting matrix  
void printMatrix(int mat[ROWS][COLUMNS]) { 
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++)
			printf("%d ", mat[i][j]);
		printf("\n");
	}
}

int main(int argc, char *argv[])
{
	//creates a composite group to physically place
	//different data types under one group
	struct timeval t1, t2;
	//creates a variable for the elapsed time 
	//with data type double  
	double Elapsed_Time;
	//Fill matrix A and matrix B using the above function 
	Fill_Matrix(MATRIX_A);
	Fill_Matrix(MATRIX_B);
    
    //start timer
	gettimeofday(&t1, NULL); 
    
    //multiplication of the 2 input matrices 
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j <COLUMNS; j++) {
			for (int k = 0; k <COLUMNS; k++) 
				MATRIX_C[i][j] += MATRIX_A[i][k] * MATRIX_B[k][j];
			}
		}
	
	//stop timer 
	gettimeofday(&t2, NULL); 

    // Compute and print the elapsed time in milli seoonds. 
	Elapsed_Time = (t2.tv_sec - t1.tv_sec)*1000.0; 
	Elapsed_Time += (t2.tv_usec - t1.tv_usec)/1000.0; 


    //print statements to print each matrix, the resultant
    //matrix and the elasped time of the program. 
	printf("Matrix A is: \n");
    printMatrix(MATRIX_A);
    printf("Matrix B is: \n");
    printMatrix(MATRIX_B);
    printf("Resultant Matrix C, where Matrix A X Matrix B is: \n");
    printMatrix(MATRIX_C);
    
    printf("The elapsed time is %f ms \n", Elapsed_Time); 
	return 0;
}

