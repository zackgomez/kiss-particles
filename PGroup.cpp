#include "PGroup.h"
#include <iostream>
#include "dprint.h"

PGroup::PGroup()
{
    action_ = NULL;
}

PGroup::~PGroup()
{
    reset();
}

Particle* PGroup::newParticle()
{
    // "allocate" a new particle
    particles_.push_back(Particle());
    return &particles_.back();
}

int PGroup::numParticles(void)
{
    return particles_.size();
}


void PGroup::setAction(PActionF* pa)
{
    if (action_)
        delete action_;
    action_ = pa;
}

void PGroup::startUpdate(float dt)
{
    update_dt_ = dt;
}

void PGroup::update()
{
    float dt = update_dt_;

    for (size_t i = 0; i < particles_.size(); )
    {
        Particle &part = particles_[i];
        if (part.t < 0)
        {
            // Use swap trick to quickly remove element
            std::swap(particles_[i], particles_.back());
            particles_.pop_back();
            // update this index again, so no increment on i
        }
        else
        {
            // apply the action
            if (action_)
                (*action_)(&part, dt);
            // Integrate
            part.update(dt);
            // Next particle
            i++;
        }
    }
}
    

void PGroup::reset(void)
{
    particles_.clear();
}

void PGroup::render(void) 
{
    renderParticles(particles_);
}

