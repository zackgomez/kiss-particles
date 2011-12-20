#include <iostream>
#include "Particle.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cstdlib>
#include <glm/glm.hpp>

void Particle::update(float dt) 
{
    loc = loc + vel * dt;
    t -= dt;
}
    
void Particle::render() 
{
    glm::mat4 transform = glm::scale(
        glm::translate(
            glm::mat4(1.0f), loc), size);
    //renderRectangle(transform, color);
}
