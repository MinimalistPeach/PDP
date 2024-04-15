#include "cpu.h"

typedef struct
{
    int start;
    int end;
    float dt;
    Particle *particles;
    float *randoms;
} ThreadData;

int numParticles = 0;
int randX, randY, randVX, randVY = 0;

void *updateParticlesThread(void *arg)
{
    ThreadData *data = (ThreadData *)arg;
    for (int i = 0; i < numParticles; i++)
    {
        data->particles[i].position.x = randX / (float)RAND_MAX;
        data->particles[i].position.y = randY / (float)RAND_MAX;
        data->particles[i].velocity.x = (randVX / (float)RAND_MAX - 0.5f) * 0.1f;
        data->particles[i].velocity.y = (randVY / (float)RAND_MAX - 0.5f) * 0.1f;
        data->randoms[2 * i] = randX / (float)RAND_MAX;
        data->randoms[2 * i + 1] = randY / (float)RAND_MAX;
    }
    for (int i = data->start; i < data->end; i++)
    {
        Particle p = data->particles[i];

        p.position.x += p.velocity.x * data->dt;
        p.position.y += p.velocity.y * data->dt;

        p.velocity.x += (data->randoms[2 * i] - 0.5f) * data->dt;
        p.velocity.y += (data->randoms[2 * i + 1] - 0.5f) * data->dt;

        data->particles[i] = p;
    }
    return NULL;
}

void startCpuParticleUpdates(int _numParticles, float dt, 
                                Particle *particles, float *randoms, 
                                int numThreads, int _randX, 
                                int _randY, int _randVX, int _randVY)
{
    pthread_t threads[numThreads];
    ThreadData thread_data[numThreads];

    int particlesPerThread = _numParticles / numThreads;

    numParticles = _numParticles;
    randX = _randX;
    randY = _randY;
    randVX = _randVX;
    randVY = _randVY;

    clock_t start = clock();

    for (int i = 0; i < numThreads; i++)
    {
        thread_data[i].start = i * particlesPerThread;
        thread_data[i].end = (i == numThreads - 1) ? _numParticles : (i + 1) * particlesPerThread;
        thread_data[i].dt = dt;
        thread_data[i].particles = particles;
        thread_data[i].randoms = randoms;
        pthread_create(&threads[i], NULL, updateParticlesThread, &thread_data[i]);
    }

    for (int i = 0; i < numThreads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    clock_t end = clock();

    printf("CPU Runtime with %d threads: %.0f ms\n", numThreads, ((double)(end-start)));
}