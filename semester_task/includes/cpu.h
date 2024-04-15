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

void startCpuParticleUpdates(int numParticle, float dt, Particle *particles, 
                                float *randoms, int numThreads, int randX, 
                                int randY, int randVX, int randVY);


#endif // CPU_H