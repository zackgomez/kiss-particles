#pragma once

#include <glm/glm.hpp>
//
// Particle system's basic particle struct.
// At some point, we may wish to add accessors/mutators
//
struct Particle
{
    // Location of the particle, in world space
    glm::vec3 loc;
    // How fast it's moving.
    glm::vec3 vel;
    // Size it should be rendered as. (currently, all particles are squares)
    glm::vec3 size;
    // Note: color includes a glow component
    glm::vec4 color;
    // How much longer will it remain alive? 
    float t; 

    // Physics calculations are performed here.
    void update(float dt);
    // Just create a matrix and render this shit to the screen.
    void render();
};
