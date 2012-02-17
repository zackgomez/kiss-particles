#include "PGroup.h"
#include <iostream>
#include <algorithm>
#include "Thread.h"

// Struct used for thread communication
struct threadarg
{
    // TODO fill this in
};

void *threadfunc(void *arg)
{
    threadarg *targ = (threadarg *) arg;

    // TODO fill the rest in
    
    // Clean up and exit thread
    delete targ;
    return 0;
}

void PGroup::update()
{
    // Use the dt from startUpdate
    float dt = update_dt_;

    // Don't do anything if there are no particles
    if (particles_.empty())
        return;

    // TODO Create a thread that does this work.
    // Create a new threadarg per thread
    // threadarg *targ = new threadarg();
    //
    // Then create and start some threads.
    //
    // Finally, wait for the threads by joining on them.
    

    // A list of dead particles that need to be removed
    std::vector<int> deadPartInds;
    // The particles and actions
    std::vector<Particle*> &particles = particles_;
    std::list<PActionF*> &actions = actions_;

    // Update/integrate each particle
    for (size_t i = 0; i < particles.size(); i++)
    {
        Particle *part = particles[i];
        // If the particle is alive update it
        if (part->t > 0)
        {
            // Update each particle first for all actions
            std::list<PActionF*>::iterator pfit;
            for (pfit = actions.begin(); pfit != actions.end(); pfit++)
            {
                (**pfit)(part, dt);
            }

            // Integrate
            part->update(dt);
        }
        // Particle is dead, add index to vector for removal
        else
        {
            deadPartInds.push_back(i);
        }
    }

    // NOTE: this part cannot be parallelized efficiently, just leave it be
    // Remove dead particles
    std::sort(deadPartInds.rbegin(), deadPartInds.rend());
    for (size_t i = 0; i < deadPartInds.size(); i++)
    {
        int pind = deadPartInds[i];
        delete particles_[pind];
        // Swap trick to quickly remove particles from a vector
        std::swap(particles_[pind], particles_.back());
        particles_.pop_back();
    }
}
    

void PGroup::render(void) 
{
    // Create a buffer of just the particle data we want and render the array
    // in one go.  It may seem slower to make this extra copy, but it's
    // actually way faster due to the way gfx cards work
    std::vector<particleData> buf(particles_.size());

    for (size_t i = 0; i < particles_.size(); i++)
    {
        buf[i].pos = particles_[i]->loc;
        buf[i].color = particles_[i]->color;
    }

    renderParticles(buf);
}

PGroup::PGroup() 
{
}

PGroup::~PGroup()
{
    reset();
}

Particle* PGroup::newParticle()
{
    // Add the particle to the "read" buffer so it gets updated on the
    // next frame
    Particle *ret = new Particle();
    particles_.push_back(ret);
    return ret;
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

void PGroup::reset(void)
{
    for (size_t i = 0; i < particles_.size(); i++)
        delete particles_[i];
    particles_.clear();
}
