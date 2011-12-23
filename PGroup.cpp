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
        //dprint((*pit)->t);
        if ((*pit)->t < 0) {
            //std::cout << "deleting particle: " << *pit << std::endl;
            delete *pit;
            pit = particles_.erase(pit);
        }
    }
    
}

int PGroup::numParticles(void)
{
    return particles_.size();
}


void PGroup::addAction(PActionF* pa)
{
    actions_.push_back(pa);
}

void PGroup::update(float dt)
{
    // apply any actions we have.
    std::list<PActionF*>::iterator pfit;
    for (pfit = actions_.begin(); pfit != actions_.end(); pfit++)
    {
        (**pfit)(particles_, dt);
    }

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
