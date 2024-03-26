#define RAND_MAX 0x7fff

typedef struct {
  float2 position;
  float2 velocity;
} Particle;

__kernel void update_particles(__global Particle *particles,
                               __global float *randoms, float dt, int randX,
                               int randY, int randVX, int randVY,
                               int NUM_PARTICLES) {
  int id = get_global_id(0);
  for (int i = 0; i < id; i++) {
    particles[i].position.x = randX / (float)RAND_MAX;
    particles[i].position.y = randY / (float)RAND_MAX;
    particles[i].velocity.x = (randVX / (float)RAND_MAX - 0.5f) * 0.1f;
    particles[i].velocity.y = (randVY / (float)RAND_MAX - 0.5f) * 0.1f;
    randoms[2 * i] = randX / (float)RAND_MAX;
    randoms[2 * i + 1] = randY / (float)RAND_MAX;
  }


  particles[id].position.x += particles[id].velocity.x * dt;
  particles[id].position.y += particles[id].velocity.y * dt;
  particles[id].velocity.x += (randoms[2 * id] - 0.5f) * dt;
  particles[id].velocity.y += (randoms[2 * id + 1] - 0.5f) * dt;

}