#include "PGroup.h"

void PGroup::render(void) 
{
    std::list<Particle *>::iterator pit;
    // loop over each particle and render
    for (pit = particles_.begin(); pit != particles_.end(); pit++)
    {
        (*pit)->render();
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
