#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define N 200

int main(int argc, char **argv) {


    int A[N][N], B[N][N], C[N][N], D[N][N];

    //Initialize
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            A[i][j] = rand() % 255;
            B[i][j] = rand() % 255;
            C[i][j] = 0 % 255;
            D[i][j] = 0 % 255;
        }
    }

    clock_t tStart = clock();
    long long sum = 0;

#pragma acc data copy(A) copy(B) create(C) create(D)
{
    //Doing it a lot of times to increase time.
    for(int l=0; l < 1000; l++){

        //Transpose C = A_T
        #pragma acc parallel loop tile(64, 32)
        for(int i=0; i<N; i++){
            for(int j=0; j<N; j++){
                C[i][j] = A[j][i];
            }
        }

        //Multiply D = CA
        #pragma acc parallel loop tile(64, 32)
        for(int i=0; i<N; i++){
            for(int j=0; j<N; j++){
                int sum = 0;
                for(int k=0; k<N; k++) {
                    sum += C[i][k] + A[k][j];
                }
                D[i][j] = sum;
            }
        }

        //Multiply C = AB
        #pragma acc parallel loop tile(64, 32)
        for(int i=0; i<N; i++){
            for(int j=0; j<N; j++){
                int sum = 0;
                for(int k=0; k<N; k++) {
                    sum += A[i][k] + B[k][j];
                }
                C[i][j] = sum;
            }
        }

        //Add A = C+D
        #pragma acc parallel loop tile(64, 32)
        for(int i=0; i<N; i++){
            for(int j=0; j<N; j++){
                A[i][j] = C[i][j] + D[i][j];
            }
        }
    }
}
    printf("Time taken: %fms\n", 1000 * (double)(clock() - tStart) / CLOCKS_PER_SEC);

    return 0;
}
