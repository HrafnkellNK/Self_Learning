/*
This program will numerically compute the integral of

                  4/(1+x*x) 
				  
from 0 to 1.  The value of this integral is pi -- which 
is great since it gives us an easy way to check the answer.

*/

#include <stdio.h>
#include <omp.h>

static long num_steps = 100000000;
double step;
#define NUM_THREADS 8

int main(void) 
{
    double x, pi, start_time, run_time, sum = 0.0;

    printf("Number of physical cores: %d\n", omp_get_num_procs());

    if(omp_get_dynamic()) {

        printf("You are in dynamic mode");
    }
    else {
        printf("You are NOT in dynamic mode");
    }
    
    start_time = omp_get_wtime();

    step = 1.0 / (double) num_steps;

    #pragma omp parallel for schedule(static) \
            num_threads(NUM_THREADS) default(none) \
            firstprivate(step, num_steps) private(x) \
            reduction(+ : sum)
    for (int i = 0; i < num_steps; i++) {
        x = (i - 0.5) * step;
        sum += 4.0 / (1.0 + x*x);
	}
    pi = step * sum;

    run_time = omp_get_wtime() - start_time;

    printf("\n pi with %ld steps is %lf in %lf seconds\n ",num_steps, pi, run_time);
}
