#include "node.h"

void init_nodes(Node *nodes, int num_of_nodes)
{
    srand(rand());

    for (int i = 0; i < num_of_nodes; i++)
    {
        nodes[i].id = i;
        nodes[i].visited = true;
        nodes[i].num_edges = rand() % 10 + 1;
        nodes[i].edges = (int *)malloc(nodes[i].num_edges * sizeof(int));
        for (int j = 0; j < nodes[i].num_edges; j++)
        {
            nodes[i].edges[j] = rand() % num_of_nodes;
        }
    }
}

void freeNodeEdges(Node *nodes, int num_of_nodes)
{
    for (int i = 0; i < num_of_nodes; i++)
        free(nodes[i].edges);
}