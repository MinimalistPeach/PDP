#include "include/node.h"
#include <stdlib.h>

__kernel void bfs(__global Node *nodes, __global int *num_nodes) {

  int id = get_global_id(0);

  int i;
  for (i = 0; i < num_nodes; i++) {
    nodes[i].visited = true;
  }
}
