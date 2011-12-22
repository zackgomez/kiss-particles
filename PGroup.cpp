#include "PGroup.h"
#include <iostream>
#include "dprint.h"

void PGroup::render(void) 
{
    //std::cout << "PGroup::render" << std::endl;
    std::list<Particle *>::iterator pit;
    // loop over each particle and render
    for (pit = particles_.begin(); pit != particles_.end(); pit++)
    {
        //std::cout << "rendering particle: " << *pit << std::endl;
        (*pit)->render();
        dprint((*pit)->t);
        if ((*pit)->t < 0) {
            //std::cout << "deleting particle: " << *pit << std::endl;
            delete *pit;
            pit = particles_.erase(pit);
        }
    }
    
}

void PGroup::update(float dt)
{
    std::list<Particle *>::iterator pit;
    for (pit = particles_.begin(); pit != particles_.end(); pit++) {
        (*pit)->update(dt);
    }
}
    

void PGroup::reset(void)
{
    std::list<Particle*>::iterator pit;
    for (pit = particles_.begin(); pit != particles_.end(); pit++) {
        delete *pit;
    }
    particles_.clear();
}
