#include "PGroup.h"
#include <iostream>

void PGroup::render(void) 
{
    // loop over each particle and render
    /*
    for (size_t i = 0; i < particles_.size(); i++)
        particles_[partInd_][i]->render();
        */

    std::vector<particleData> buf(particles_[partInd_].size());

    for (size_t i = 0; i < particles_[partInd_].size(); i++)
    {
        buf[i].pos = particles_[partInd_][i]->loc;
        buf[i].color = particles_[partInd_][i]->color;
    }

    renderParticles(buf);
}

PGroup::PGroup() : partInd_(0)
{

}

PGroup::~PGroup()
{
    reset();
}

Particle* PGroup::newParticle()
{
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
    float dt = update_dt_;

    // apply any actions we have.
    std::list<PActionF*>::iterator pfit;
    for (pfit = actions_.begin(); pfit != actions_.end(); pfit++)
    {
        (**pfit)(particles_[partInd_], dt);
    }

    for (size_t i = 0; i < particles_[partInd_].size(); )
    {
        Particle *part = particles_[partInd_][i];
        if (part->t < 0)
        {
            delete part;
            // Use swap trick to quickly remove element
            std::swap(particles_[partInd_][i], particles_[partInd_].back());
            particles_[partInd_].pop_back();
            // update this index again, so no increment on i
        }
        else
        {
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
