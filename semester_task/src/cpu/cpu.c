#include "cpu.h"

void cpu_start(Node *nodes, int number_of_nodes, int number_of_threads)
{
    double seq_runtime;
    seq_runtime = bfsSequential(nodes, number_of_nodes);
    printf("%d db csomopont eseten a szekvencialis futasi ido: %f ms\n", number_of_nodes, seq_runtime);

    double parallel_runtime;
    parallel_runtime = startThreads(nodes, number_of_threads, number_of_nodes);
    printf("%d db csomopont eseten %d szalon a futasi ido: %f ms\n", number_of_nodes, number_of_threads, parallel_runtime);

    int num_of_visited_nodes;
    num_of_visited_nodes = countVisitedNodes(nodes, number_of_nodes);
    printf("Bejart csomopontok szama: %d\n", num_of_visited_nodes);

    freeNodeEdges(nodes, number_of_nodes);

    free(nodes);
}