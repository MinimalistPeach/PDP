#ifndef GPU_H
#define GPU_H

#include "particle.h"
#include "kernel_loader.h"

#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>
#include <CL/cl_platform.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int startGpuParticleUpdates(int numParticles, float dt, Particle *particles, 
                                float *randoms, int randX, int randY, 
                                int randVX, int randVY);


#endif // GPU_H