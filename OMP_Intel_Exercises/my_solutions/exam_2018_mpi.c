

/* ================== REGULAR PROBLEM 9 ==================== */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

// int main ( int argc , char ** argv )
// {
//     int value , temp ;
//     int size , rank , row , col ;
//     MPI_Comm c1 , c2 , c3 ;

//     MPI_Init (& argc , & argv );

//     MPI_Comm_size ( MPI_COMM_WORLD , & size );
//     MPI_Comm_rank ( MPI_COMM_WORLD , & rank );


//     col = rank % 4;
//     row = rank / 4;

//     MPI_Comm_split ( MPI_COMM_WORLD , row , col , & c1 );
//     MPI_Comm_split ( MPI_COMM_WORLD , col , row , & c2 );
//     MPI_Comm_split ( MPI_COMM_WORLD , rank , col , & c3 );

    
//     MPI_Comm_rank ( c1 , & value );
//     MPI_Comm_rank ( c2 , & value );


//     MPI_Allreduce (& rank , & value , 1 , MPI_INT , MPI_MAX , c3 );
//     temp = value ;

//     MPI_Allreduce (& temp , & value , 1 , MPI_INT , MPI_SUM , c2 );
//     temp = value ;

//     MPI_Allreduce (& temp , & value , 1 , MPI_INT , MPI_MIN , c1 );
//     printf("%d - %d\n", rank ,value);

//     MPI_Finalize ();
// }


/* ============================ REGULAR PROBLEM 10 ======================== */

// function to initialize two of size n
// void init ( float *x , float * y , int n ){}

// // very expensive function which only works on elements of input array x
// void costly_calculation ( float * x , int n ){}

// int main ( int argc , char * argv [])
// {
//     int rank , size ;

//     MPI_Init (& argc , & argv );
//     MPI_Comm_rank ( MPI_COMM_WORLD , & rank );
//     MPI_Comm_size ( MPI_COMM_WORLD , & size );

//     // float * a = ( float *) malloc ( size * sizeof ( float ));
//     // float * b = ( float *) malloc ( size * sizeof ( float ));
//     float *a = (float *)calloc(size, sizeof(float));
//     float *b = (float *)calloc(size, sizeof(float));

//     // init(a, b, size);
//     float data = rank;

//     // point to point communication
//     // if ( rank == 0)
//     // {
//     //     for ( int r = 1; r < size ; r ++)
//     //     {
//     //         a [0] = data ;
//     //         MPI_Recv ( a + r , 1 , MPI_FLOAT , r , 0 , MPI_COMM_WORLD , MPI_STATUS_IGNORE );
//     //     }
//     // } 
//     // else
//     //     MPI_Send (& data , 1 , MPI_FLOAT , 0 , 0 , MPI_COMM_WORLD );

//     // MPI_Gather(&data, 1, MPI_FLOAT, a, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

//     MPI_Request req;
//     MPI_Igather(&data, 1, MPI_FLOAT, a, 1, MPI_FLOAT, 0, MPI_COMM_WORLD, &req);
//     MPI_Wait(&req, MPI_STATUS_IGNORE);

//     if(rank == 0) {
//         for(int i = 0; i < size; i++)
//         {
//             printf("%f ", a[i]);
//         }
//         printf("\n");
//     }

//     // costly_calculation (b , size );
//     // costly_calculation (a , size );

//     free ( a ); free ( b );

//     MPI_Finalize ();
//     return 0;
// }

// ================================ RETAKE PROBLEM 9 ================================================

int main ( int argc , char ** argv )
{
    int value, temp;
    int size , rank , col [4];
    MPI_Comm c [4];

    MPI_Init (& argc , & argv );
    MPI_Comm_size ( MPI_COMM_WORLD , & size );
    MPI_Comm_rank ( MPI_COMM_WORLD , & rank );

    col[0] = rank & (1 << 3);
    col[1] = rank & (1 << 2);
    col[2] = rank & (1 << 1);
    col[3] = rank & (1 << 0);

    // if(rank == 0)
    //     printf("%d\n", 4&4);
    // printf("Process nr.%d - col[0] = %d\n", rank, col[2]);

    MPI_Comm_split(MPI_COMM_WORLD, col[0], rank, c);
    MPI_Comm_split(c[0],           col[1], rank, c+1);
    MPI_Comm_split(c[1],           col[2], rank, c+2);
    MPI_Comm_split(c[2],           col[3], rank, c+3);

    MPI_Comm_rank(c[1], &temp);
    // printf("Process nr.%d -> nr.%d\n", rank, temp);

    MPI_Comm_rank ( c [0] , & value );
    if ( rank == 7) printf ( " Value ␣ 1: ␣ % i \n " , value );

    MPI_Comm_rank ( c [1] , & value );
    if ( rank == 8) printf ( " Value ␣ 2: ␣ % i \n " , value );

    MPI_Comm_size ( c [2] , & value );
    if ( rank == 9) printf ( " Value ␣ 3: ␣ % i \n " , value );

    MPI_Allreduce (& rank , & value , 1 , MPI_INT , MPI_MIN , c [3]);
    if ( rank == 10) printf ( " Value ␣ 4: ␣ % i \n " , value );
    temp = value;

    MPI_Allreduce (& temp , & value , 1 , MPI_INT , MPI_SUM , c [1]);
    if ( rank == 11) printf ( " Value ␣ 5: ␣ % i \n " , value );
    temp = value ;

    MPI_Allreduce (& temp , & value , 1 , MPI_INT , MPI_MAX , c [2]);
    if ( rank == 12) printf ( " Value ␣ 6: ␣ % i \n " , value );

    MPI_Finalize ();
}