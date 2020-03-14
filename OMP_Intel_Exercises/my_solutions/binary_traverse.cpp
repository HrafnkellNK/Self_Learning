/*
Para F5 stencil example with and without thread binding. 
Test would be fun!

Compile:
g++ binary_traverse.cpp -o build/run -std=c++11 -Wall -fopenmp -fstrict-overflow -pedantic -O0 -Werror -Wshadow -Wstrict-overflow=5
 */


#include <iostream>
#include <omp.h>
#include <unistd.h>

void process(struct node *p);

struct node
{
    struct node *left, *right;
    int key;
    node(int k): key(k){}
};

void process(struct node *p) 
{
    usleep(1000000);

    std::cout << "element with key: "
              << p->key << " is processed"
              << std::endl;
}

void traverse(struct node *p)
{
    #pragma omp parallel sections num_threads(8)
    {
        #pragma omp section
        {
            if (p->left != NULL)
                traverse(p->left);
        }

        #pragma omp section
        {
            if (p->right != NULL)
                traverse(p->right);
        }
    }

    process(p);
}

int main(int argc, char** argv)
{
    struct node *tree = new struct node (0);

    tree->left = new struct node(1);
    tree->right = new struct node(2);
    tree->left->left = new struct node(3);
    tree->left->right = new struct node(4);
    tree->right->left = new struct node(5);
    tree->right->right = new struct node(6);    
    
    traverse(tree);

    return 0;
}