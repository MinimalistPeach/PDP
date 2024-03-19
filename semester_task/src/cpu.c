#include "cpu.h"

typedef struct
{
    int start;
    int end;
    float dt;
    Particle *particles;
    float *randoms;
} ThreadData;

int NUM_PARTICLES = 0;
int randX, randY, randVX, randVY = 0;

void *update_particles_thread(void *arg)
{
    ThreadData *data = (ThreadData *)arg;
    for (int i = 0; i < NUM_PARTICLES; i++)
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

void start_cpu_particle_updates(int _num_particles, float dt, Particle *particles, float *randoms, int num_threads, int _randX, int _randY, int _randVX, int _randVY)
{
    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];

    int particles_per_thread = _num_particles / num_threads;

    NUM_PARTICLES = _num_particles;
    randX = _randX;
    randY = _randY;
    randVX = _randVX;
    randVY = _randVY;

    clock_t start = clock();
    for (int i = 0; i < num_threads; i++)
    {
        thread_data[i].start = i * particles_per_thread;
        thread_data[i].end = (i == num_threads - 1) ? _num_particles : (i + 1) * particles_per_thread;
        thread_data[i].dt = dt;
        thread_data[i].particles = particles;
        thread_data[i].randoms = randoms;
        pthread_create(&threads[i], NULL, update_particles_thread, &thread_data[i]);
    }

    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);

        clock_t end = clock();
        printf("CPU %d thread runtime: %.0f ms\n", i+1, (double)(end - start));
    }
}