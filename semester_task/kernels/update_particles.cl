#define RAND_MAX 0x7fff

typedef struct {
  float2 position;
  float2 velocity;
} Particle;

__kernel void update_particles(__global Particle *particles,
                               __global float *randoms, float dt, int randX,
                               int randY, int randVX, int randVY, int NUM_PARTICLES) {
  for (int i = 0; i < NUM_PARTICLES; i++) {
    particles[i].position.x = randX / (float)RAND_MAX;
    particles[i].position.y = randY / (float)RAND_MAX;
    particles[i].velocity.x = (randVX / (float)RAND_MAX - 0.5f) * 0.1f;
    particles[i].velocity.y = (randVY / (float)RAND_MAX - 0.5f) * 0.1f;
    randoms[2 * i] = randX / (float)RAND_MAX;
    randoms[2 * i + 1] = randY / (float)RAND_MAX;
  }

  int id = get_global_id(0);
  Particle p = particles[id];

  p.position.x += p.velocity.x * dt;
  p.position.y += p.velocity.y * dt;

  p.velocity.x += (randoms[2 * id] - 0.5f) * dt;
  p.velocity.y += (randoms[2 * id + 1] - 0.5f) * dt;

  particles[id] = p;
}