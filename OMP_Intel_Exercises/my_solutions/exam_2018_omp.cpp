#include <stdio.h>
#include <omp.h>

// =============================== EXAM REGULAR ====================================

void prob3_a ( float *a , float *b , int n )
{
    int i;

    #pragma omp parallel for schedule(static) lastprivate(i) 
    for (i = 0; i < n - 1; i ++)
        a [ i ] = ( b [ i ] + b [ i + 1]) / 2;

    a [ i ] = b[ i ] / 2;
}

double prob3_b ( float *x , float *y , int n )
{
    int i , b = y [0];
    float a = 1.0;

    #pragma omp parallel for schedule(static) reduction(+ : a)
    for ( i =0; i < n ; i ++)
    {
        a += x [ i ];

        if ( b < y [ i ]) {
            b = y [ i ];
        }
    }

    printf("%d\n", b);
    return a * b ;
}

void prob4_a() 
{
    int value [64] , k , t ;

    omp_set_num_threads(16);

    for ( int i =0; i <64; i ++)
        value [ i ]=63 - i;

    t = omp_get_thread_num ();
    k =42;

    # pragma omp parallel for schedule ( static , 2) firstprivate(k, t) lastprivate(k, t)
    for ( int i =0; i <64; i ++)
    {
        k = value [ i ]*2+ t;
    }

    printf ( " Final ␣ value ␣ of ␣ k =% i \n " , k );
}

void prob6_a ()
{
    omp_set_num_threads(4);

    const int N = 1000000;
    int a [ N ];
    // int b [ N ];

    for ( int i = 0; i < N ; i ++)
        a[i] = i;

    # pragma omp parallel for schedule ( static , 1)
    for ( int i = 0; i < N ; i ++)
    {
        a [ i ] = a [ i ] + 17;
        // b[i] = a [ i ] % 23;
    }
}


// ======================= RETAKE EXAM =======================================

void retake_prob3_a( float *a , float *b , int n )
{
    int i , j = 5;

    #pragma omp parallel for schedule(static)
    for ( i = 0; i < n ; i ++)
    {
        int k = i + j + n /4;
        a [ i ] = b [ k % n ] / 2;
    }

    for (int x = 0; x < n; x++)
    {
        printf("%f\n", a[x]);
    }
    
}

# define f( A ) (4.0 / (1.0 + A * A ))

double retake_prob3_b ( int n )
{
    double x ;
    double w = 1.0/ n ;
    double pi = 0;

    #pragma omp parallel for schedule(static) private(x) reduction(+ : pi)
    for ( int i = 1; i <= n ; i ++)
    {
        x = w * (( double ) i - 0.5);
        pi = pi + w * f ( x );
    }

    return pi;
}

void retake_prob4_a ()
{

    omp_set_num_threads(5);
    int value [30] , k , t ;

    for ( int i =2; i <32; i ++)
        value [i -2]= i ;

    k = -1;
    t = 0;

    # pragma omp parallel for schedule ( static , 3) lastprivate(k, t)
    for ( int i =2; i <32; i ++)
    {
        t = omp_get_thread_num ();
        k = value [i -2]*2+ t ;
    }

    printf ( " Final ␣ value ␣ of ␣ k =% i \n " , k );
}

int main(void) 
{
    double start_time, run_time;
    

    // float arr_a[10] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    // float arr_b[10] = {10, 20, 300, 90, 500, 0, 700, 55, 900, 1000};

    start_time = omp_get_wtime();
    // prob3_a(&arr_a[0], &arr_b[0], 10);
    // double sum = prob3_b(&arr_a[0], &arr_b[0], 10);
    // prob4_a();
    // prob6_a();
    // retake_prob3_a(&arr_a[0], &arr_b[0], 10);
    // printf("%lf\n", retake_prob3_b(999999999));
    retake_prob4_a();
    run_time = omp_get_wtime() - start_time;


    printf("%lf seconds\n ", run_time);
    
}
