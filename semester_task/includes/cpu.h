#ifndef CPU_H
#define CPU_H

#include "particle.h"
#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#define RAND_MAX 0x7FFF

typedef struct
{
    clock_t start;
    clock_t end;
} CPURunningTime;

void start_cpu_particle_updates(int num_particles, float dt, Particle *particles, float *randoms, int num_threads, int randX, int randY, int randVX, int randVY);


#endif // CPU_H