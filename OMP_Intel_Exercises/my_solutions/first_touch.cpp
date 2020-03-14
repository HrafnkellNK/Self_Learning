/*
Para F5 stencil example with and without thread binding. 
Test would be fun!
 */


#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 5000
#define ITER 100

double A[N+2][N+2];

int main(int argc, char** argv)
{
    double starttime, endtime;
    int threads = 1;

    // trying to utilize "First Touch" policy of openmp
    // supposed to bind threads to different processors
    #pragma omp parallel for
    for (int i=0; i<N+2; i++)
        for (int j=0; j<N+2; j++)
            A[i][j]=0.0;

    for (int i=0; i<N+2; i++)
        A[i][0]=1.0;

    for (int i=0; i<N+2; i++)
        A[i][N+2]=1.0;

    starttime = omp_get_wtime();

    //Iteration
    for (int n=0; n<100; n++)
    {
        #pragma omp parallel num_threads(8) default(none) shared(A, threads)
        {
            threads=omp_get_num_threads();

            #pragma omp for
            for (int i=1; i<N+1; i++)
                for (int j=1; j<N+1; j++)
                    A[i][j]=(A[i+1][j]+A[i-1][j]+A[i][j-1]+A[i][j+1])/4.0;
        }
    }

    endtime=omp_get_wtime();

    printf("threads: %i time %f\n", threads, endtime-starttime);

   return 0;
}