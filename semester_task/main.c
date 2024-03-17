#include "gpu.h"
#include "cpu.h"

#define NUM_PARTICLES 100000
const float DT = 2.5f;

int main(void)
{
    srand(time(NULL));
    Particle *particles = (Particle *)malloc(NUM_PARTICLES * sizeof(Particle));
    float *randoms = (float *)malloc(2 * NUM_PARTICLES * sizeof(float));

    int randX = rand();
    int randY = rand();
    int randVX = rand();
    int randVY = rand();

    start_cpu_particle_updates(NUM_PARTICLES, DT, particles, randoms, 4, randX, randY, randVX, randVY);
    start_gpu_particle_updates(NUM_PARTICLES, DT, particles, randoms, randX, randY, randVX, randVY);

    free(particles);
}
