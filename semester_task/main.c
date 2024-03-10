#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "node.h"
#include "bfs.h"
#include "cpu.h"
#include "gpu.h"

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

    //gpu_start(nodes, number_of_nodes);
    cpu_start(nodes, number_of_nodes, number_of_threads);

    return 0;
}