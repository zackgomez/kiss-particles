#include <iostream>
#include "Particle.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cstdlib>
#include <glm/glm.hpp>
#include <GL/glut.h>

void Particle::update(float dt) 
{
    loc = loc + vel * dt;
    t -= dt;
}
    
void Particle::render() 
{
    std::cout << "rendering" << std::endl;
    glm::mat4 transform = glm::scale(
        glm::translate(
            glm::mat4(1.0f), loc), size);
    
    // TODO: render something (anything!) here
    //glutSolidSphere(1112.0, 2, 2);
    //renderRectangljke(transform, color);
}
