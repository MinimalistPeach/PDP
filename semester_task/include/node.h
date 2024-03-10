#ifndef NODE_H
#define NODE_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#define MAX_NUM 10000000

typedef struct {
    int id;
    int* edges;
    int num_edges;
    bool visited;
} Node;

/*
 * Node initalization.
*/
void init_nodes(Node *nodes, int num_of_nodes);

/*
 * Free up the allocated memory for the node edges.
*/
void freeNodeEdges(Node *nodes, int num_of_nodes);



#endif /*NODE_H*/