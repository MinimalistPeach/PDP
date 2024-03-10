#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "node.h"
#include "bfs.h"

int main(int argc, char *argv[])
{
    int number_of_threads;
    int number_of_nodes;

    if (argc != 3)
    {
        printf("\nHasznalat: bfs_alg.exe <csomopontok szama> <szalak szama>");
        return 1;
    }
    else
    {
        if (atoi(argv[1]) > MAX_NUM)
        {
            printf("Maximum megadhato szam: %d", MAX_NUM);
            return 1;
        }
        else
        {
            number_of_nodes = atoi(argv[1]);
        }
        number_of_threads = atoi(argv[2]);
    }
    Node *nodes = malloc(sizeof(Node) * number_of_nodes);

    init_nodes(nodes, number_of_nodes);

    double seq_runtime;
    seq_runtime = bfsSequential(nodes, number_of_nodes);
    printf("%d db csompont eseten a szekvencialis futasi ido: %f ms\n", number_of_nodes, seq_runtime);

    double parallel_runtime;
    parallel_runtime = startThreads(nodes, number_of_threads, number_of_nodes);
    printf("%d db csomopont eseten %d szalon a futasi ido: %f ms\n", number_of_nodes, number_of_threads, parallel_runtime);

    int num_of_visited_nodes;
    num_of_visited_nodes = countVisitedNodes(nodes, number_of_nodes);
    printf("Bejart csomopontok szama: %d\n", num_of_visited_nodes);

    freeNodeEdges(nodes, number_of_nodes);

    free(nodes);

    return 0;
}