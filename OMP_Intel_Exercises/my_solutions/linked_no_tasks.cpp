#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#ifndef N
#define N 5
#endif
#ifndef FS
#define FS 38
#endif

struct node {
   int data;
   int fibdata;
   struct node* next;
};

int fib(int n) {
   int x, y;
   if (n < 2) {
      return (n);
   } else {
      x = fib(n - 1);
      y = fib(n - 2);
	  return (x + y);
   }
}

void processwork(struct node* p) 
{
   int n;
   n = p->data;
   p->fibdata = fib(n);
}

struct node* init_list(struct node* p) {
    int i;
    struct node* head = NULL;
    struct node* temp = NULL;
    
    head = (struct node*)malloc(sizeof(struct node));
    p = head;
    p->data = FS;
    p->fibdata = 0;
    for (i=0; i< N; i++) {
       temp  =  (struct node*)malloc(sizeof(struct node));
       p->next = temp;
       p = temp;
       p->data = FS + i + 1;
       p->fibdata = i+1;
    }
    p->next = NULL;
    return head;
}

int main(int argc, char *argv[]) {
    double start, end;
    struct node *p=NULL;
    struct node *temp=NULL;
    struct node *head=NULL;
    struct node *nodes[N+1];
     
    printf("Process linked list\n");
    printf("  Each linked list node will be processed by function 'processwork()'\n");
    printf("  Each ll node will compute %d fibonacci numbers beginning with %d\n",N,FS);      
 
    p = init_list(p);
    head = p;

    // create array of pointers, each elem points to node
    int index = 0;
    while(p != NULL) {
        nodes[index] = p;
        p = p->next;
        index++;
    }

    start = omp_get_wtime();

    // parallize for loop using the array of nodes instead
    #pragma omp parallel for schedule(dynamic, 1) \
            default(none) shared(nodes) num_threads(8)
    for (int i = 0; i < N+1; i++)
    {
        processwork(nodes[i]);
    }
    

    end = omp_get_wtime();

    np = head;
    while (p != NULL) {
        printf("%d : %d\n",p->data, p->fibdata);
        temp = p->next;
        free (p);
        p = temp;
    }  
    free (p);

    printf("Compute Time: %f seconds\n", end - start);

    return 0;
}

