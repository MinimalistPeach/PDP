#include "gpu.h"
#include "cpu.h"

const float DT = 1.2f;

int main(int argc, char* argv[])
{

    if(argc != 3)
    {
        printf("Usage: %s <num_threads> <num_particles>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[1]);
    int NUM_PARTICLES = atoi(argv[2]);


    srand(time(NULL));
    Particle *particles = (Particle *)malloc(NUM_PARTICLES * sizeof(Particle));
    float *randoms = (float *)malloc(2 * NUM_PARTICLES * sizeof(float));

    int randX = rand();
    int randY = rand();
    int randVX = rand();
    int randVY = rand();

    start_cpu_particle_updates(NUM_PARTICLES, DT, particles, randoms, num_threads, randX, randY, randVX, randVY);
    start_gpu_particle_updates(NUM_PARTICLES, DT, particles, randoms, randX, randY, randVX, randVY);

    free(particles);
}
