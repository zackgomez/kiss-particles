#include "PGroup.h"
#include <iostream>
#include "Thread.h"

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

// Struct used for thread communication
struct threadarg
{
    // TODO fill this in
};

void *threadfunc(void *arg)
{
    threadarg *targ = (threadarg *) arg;

    // TODO fill the rest in
    
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

    // TODO Create a thread that does this work.  You will probably need a 
    // struct to pass to each thread.
    //
    // Then create and start some threads.
    //
    // Finally, wait for the threads by joining on them.
    
    // Update each particle first for any actions, then integrate position
    std::list<PActionF*>::iterator pfit;
    for (pfit = actions_.begin(); pfit != actions_.end(); pfit++)
    {
        (**pfit)(particles_, dt);
    }
    // Update/integrate each particle
    for (size_t i = 0; i < particles_.size(); i++)
        particles_[i]->update(dt);

    // Remove dead particles
    // NOTE this cannot be parallelized...
    for (size_t i = 0; i < particles_.size(); )
    {
        Particle *part = particles_[i];
        if (part->t < 0)
        {
            delete part;
            std::swap(particles_[i], particles_.back());
            particles_.pop_back();
        }
        else
        {
            i++;
        }
    }
}
    

void PGroup::reset(void)
{
    for (size_t i = 0; i < particles_.size(); i++)
        delete particles_[i];
    particles_.clear();
}
