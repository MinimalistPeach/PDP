#ifndef BFS_H
#define BFS_H

#include "node.h"
#include <pthread.h>

typedef struct {
    int start;
    int end;
    Node* nodes;
} ThreadData;

/*
 * Parallel graph algorithm.
*/
void* bfsParallel(void* args);

/*
 * Sequential graph algorithm.
*/
double bfsSequential(Node* nodes, int num_nodes);

/*
 * Counts the visited nodes.
*/
int countVisitedNodes(Node* nodes, int num_nodes);

/*
 * Starts and shuts down the threads for the parallelization.
*/
double startThreads(Node* nodes, int number_of_threads, int number_of_nodes);

/*
 * Free the allocated memory of the threads and thread data.
*/
void freeThreads(pthread_t *threads, ThreadData *threaddata, int num_of_nodes);

#endif /*BFS_H*/