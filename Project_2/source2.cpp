/*----------------------------------------------------------------------------------
 * File:        Lab_02_2.c
 * Description: This program performs the matrix multiplication on two 8x8 matrices.
 * Author:      Min Han
 * Date:        August 28, 2022
 *----------------------------------------------------------------------------*/
#include <stdio.h>

#define N 8



void matrixMultiply(float A[N][N], float B[N][N], float C[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0.0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}




int main() {
    float matrixA[N][N] = {
        {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0},

    };

    float matrixB[N][N] = {
        {8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0},

    };

    float resultMatrix[N][N];

    matrixMultiply(matrixA, matrixB, resultMatrix);

    printf("Matrix A:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%.2f ", matrixA[i][j]);
        }
        printf("\n");
    }

    printf("\nMatrix B:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%.2f ", matrixB[i][j]);
        }
        printf("\n");
    }

    printf("\nResult Matrix:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%.2f ", resultMatrix[i][j]);
        }
        printf("\n");
    }

    return 0;
}
