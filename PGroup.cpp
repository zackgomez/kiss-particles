#include "PGroup.h"
#include <iostream>

void PGroup::render(void) 
{
    // Create a buffer of just the particle data we want and render the array
    // in one go.  It may seem slower to make this extra copy, but it's
    // actually way faster due to the way gfx cards work
    std::vector<particleData> buf(particles_[partInd_].size());

    for (size_t i = 0; i < particles_[partInd_].size(); i++)
    {
        buf[i].pos = particles_[partInd_][i]->loc;
        buf[i].color = particles_[partInd_][i]->color;
    }

    renderParticles(buf);
}

PGroup::PGroup() :
    // Start out reading from 0 and writing to 1
    partInd_(0)
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
    particles_[partInd_].push_back(ret);
    return ret;
}

int PGroup::numParticles(void)
{
    return particles_[partInd_].size();
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
    // Use the dt from startUpdate
    float dt = update_dt_;

    // apply any particle actions we have (gravity, other forces)
    std::list<PActionF*>::iterator pfit;
    for (pfit = actions_.begin(); pfit != actions_.end(); pfit++)
    {
        (**pfit)(particles_[partInd_], dt);
    }

    // Loop over each particle removing it if necessary, and updating
    // otherwise
    for (size_t i = 0; i < particles_[partInd_].size(); )
    {
        Particle *part = particles_[partInd_][i];
        if (part->t < 0)
        {
            delete part;
            // Use swap trick to quickly remove element
            std::swap(particles_[partInd_][i], particles_[partInd_].back());
            particles_[partInd_].pop_back();
            // new particle in index, don't update i
        }
        else
        {
            // Particle is still alive, just update and move on
            part->update(dt);
            i++;
        }
    }
}
    

void PGroup::reset(void)
{
    for (partInd_ = 0; partInd_ < 2; partInd_++)
    {
        for (size_t i = 0; i < particles_[partInd_].size(); i++)
            delete particles_[partInd_][i];
        particles_[partInd_].clear();
    }
}
