#include "gpu.h"
#include "cpu.h"

const float DT = 1.2f;

int main(int argc, char* argv[])
{

    if(argc != 3)
    {
        printf("Usage: %s <numThreads> <numParticles>\n", argv[0]);
        return 1;
    }

    int numThreads = atoi(argv[1]);
    int numParticles = atoi(argv[2]);


    srand(time(NULL));
    Particle *particles = (Particle *)malloc(numParticles * sizeof(Particle));
    float *randoms = (float *)malloc(2 * numParticles * sizeof(float));

    int randX = rand();
    int randY = rand();
    int randVX = rand();
    int randVY = rand();

    startCpuParticleUpdates(numParticles, DT, particles, randoms, numThreads, randX, randY, randVX, randVY);
    startGpuParticleUpdates(numParticles, DT, particles, randoms, randX, randY, randVX, randVY);

    free(particles);
}
