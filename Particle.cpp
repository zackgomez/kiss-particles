#include "Particle.h"

void renderParticle(const glm::vec3 &loc,
        const glm::vec3& part_size, const glm::vec4 &part_color);

void Particle::update(float dt) 
{
    loc = loc + vel * dt;
    t -= dt;
}
