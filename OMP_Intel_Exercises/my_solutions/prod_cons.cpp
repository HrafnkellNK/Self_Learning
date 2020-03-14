/*
**  PROGRAM: A simple serial producer/consumer program
**
**  One function generates (i.e. produces) an array of random values.  
**  A second functions consumes that array and sums it.
**
**  HISTORY: Written by Tim Mattson, April 2007.
*/
#include <omp.h>
#ifdef APPLE
#include <stdlib.h>
#else
#include <malloc.h>
#endif
#include <stdio.h>

#define N        10000

/* Some random number constants from numerical recipies */
#define SEED       2531
#define RAND_MULT  1366
#define RAND_ADD   150889
#define RAND_MOD   714025
int randy = SEED;

/* function to fill an array with random numbers */
void fill_rand(int length, double *a)
{
    int i; 
    for (i=0;i<length;i++) {
        randy = (RAND_MULT * randy + RAND_ADD) % RAND_MOD;
        *(a+i) = ((double) randy)/((double) RAND_MOD);
    }   
}

/* function to sum the elements of an array */
double Sum_array(int length, double *a)
{
    int i;  double sum = 0.0;
    for (i=0;i<length;i++)  sum += *(a+i);  
    return sum; 
}
  
int main()
{
    // FYI - people in the comments diagreed that this is a parallel solution
    // Should try to make it purely parallel
    // Here are the comments:  https://www.youtube.com/watch?v=WcPZLJKtywc&list=PLLX-Q6B8xqZ8n8bwjGdzBJ25X2utwnoEG&index=8
    double *A, sum, runtime;
    int flag = 0, flag_temp;

    A = (double *)malloc(N*sizeof(double));

    runtime = omp_get_wtime();

    #pragma omp parallel sections num_threads(2)
    {
        // make sure only one thread executes this section
        // aka this is the producer
        #pragma omp section
        {
            fill_rand(N, A);        // Producer: fill an array of data

            // make sure that array-A is visible to consumer thread in the shared memory
            #pragma omp flush

            // let consumer know that array is ready to be consumed
            // TODO: learn aboutfancy syncing with atomic write
            #pragma omp atomic write
            flag = 1;

            // make sure that flag variable is updated
            #pragma omp flush(flag)
        }

        #pragma omp section
        {
            // spin lock
            while(true) {
                // compiler might put flag into registary 
                // keep checking to see if flag has been updated
                #pragma omp flush(flag)
                
                #pragma omp atomic read
                flag_temp = flag;

                if(flag_temp == 1) break;
            }

            // make sure we are working with updated A
            #pragma omp flush

            sum = Sum_array(N, A);  // Consumer: sum the array
        }
    }

    runtime = omp_get_wtime() - runtime;

    printf(" In %f seconds, The sum is %f \n",runtime,sum);
}
 
