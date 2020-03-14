/*
This program will numerically compute the integral of

                  4/(1+x*x) 
				  
from 0 to 1.  The value of this integral is pi -- which 
is great since it gives us an easy way to check the answer.

The is the original sequential program.  It uses the timer
from the OpenMP runtime library
*/



// ========================= NON-PARALLEL =============================
// #include <stdio.h>
// #include <omp.h>

// static long num_steps = 100000000;
// double step;

// int main(void) 
// {
//     int i; 
//     double x, pi, start_time, run_time, sum = 0.0;

//     start_time = omp_get_wtime();

//     step = 1.0 / (double) num_steps;

//     for (i = 1; i <= num_steps; i++) {
//         x = (i - 0.5) * step;
//         sum = sum + 4.0 / (1.0 + x*x);
// 	}
//     pi = step * sum;

//     run_time = omp_get_wtime() - start_time;

//     printf("\n pi with %ld steps is %lf in %lf seconds\n ",num_steps, pi, run_time);
// }


// g++ pi_mc.cpp random.cpp -o build/run -std=c++11 -Wall -fopenmp -fstrict-overflow -pedantic -O0 -Werror -Wshadow -Wstrict-overflow=5

// ============================= PARALLEL ================================

#include <stdio.h>
#include <omp.h>

#define NUM_THREADS 8
static long num_steps = 100000000;

int main(void) 
{
    // create sum array so each thread can work on its own calculations per element
    double sum[NUM_THREADS];
    double step = 1.0 / (double) num_steps;

    // Request NUM_THREADS threads, BUT the computer 
    // can decide to give you FEWER threads than requested
    omp_set_num_threads(NUM_THREADS);

    // so we need to have a shared variable 
    // and ask the master thread in the parallel
    // region how many threads there really are
    int real_num_threads;

    double start_time = omp_get_wtime();
    #pragma omp parallel default(none) \
            shared(sum, real_num_threads) firstprivate(num_steps, step)
    {
        int ID = omp_get_thread_num();

        // ask for the real number of threads
        real_num_threads = omp_get_num_threads();

        int step_size = num_steps / real_num_threads;

        // calculate where each thread should iterate to
        int end = ID < (real_num_threads - 1) ? (ID + 1) * step_size : (ID + 1) * step_size + (num_steps % real_num_threads);
        double x, temp_result = 0.0;
        
        printf("Thread %d from %d to %d \n", ID, (ID * step_size), end);

        // each thread calculates different iteration span 
        for (int i = (ID * step_size); i < end; i++) {
            x = (i - 0.5) * step;
            temp_result += + 4.0 / (1.0 + x*x);
        }

        // array access is expensive! so we use temporary result variable
        // so each element is only accessed once per thread
        // also avoids false sharing
        sum[ID] = temp_result;
    }
    
    double pi = 0.0;
    for (int i = 0; i < real_num_threads; i++)
    {
        pi += sum[i] * step;
    }

    double run_time = omp_get_wtime() - start_time;

    printf("\n pi with %ld steps is %lf in %lf seconds\n ",num_steps, pi, run_time);
}
