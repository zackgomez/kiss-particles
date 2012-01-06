#include "PGroup.h"
#include <iostream>
#include "dprint.h"
 
void PGroup::render(void) 
{
    std::list<Particle *>::iterator pit;
    // loop over each particle and render
    for (pit = particles_.begin(); pit != particles_.end(); pit++)
    {
        (*pit)->render();
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

void PGroup::startUpdate(float dt)
{
    update_dt_ = dt;
}

void PGroup::update()
{
    float dt = update_dt_;

    // apply any actions we have.
    std::list<PActionF*>::iterator pfit;
    for (pfit = actions_.begin(); pfit != actions_.end(); pfit++)
    {
        (**pfit)(particles_, dt);
    }

    // Run each particle's update
    std::list<Particle *>::iterator pit;
    for (pit = particles_.begin(); pit != particles_.end(); ) {
        // first check for removal
        if ((*pit)->t < 0) {
            delete *pit;
            pit = particles_.erase(pit);
        }
        else
		{
            // now update
            (*pit)->update(dt);
			pit++;
		}
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
