#include "PGroup.h"
#include <iostream>
#include "dprint.h"

void PGroup::render(void) 
{
    // loop over each particle and render
    /*
    for (size_t i = 0; i < particles_.size(); i++)
        particles_[i]->render();
        */

    std::vector<particleData> buf(particles_.size());

    for (size_t i = 0; i < particles_.size(); i++)
    {
        buf[i].pos = particles_[i]->loc;
        buf[i].color = particles_[i]->color;
    }

    renderParticles(buf);
}

PGroup::~PGroup()
{
    reset();
}

Particle* PGroup::newParticle()
{
    // TODO use something more intricate
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

void PGroup::update()
{
    float dt = update_dt_;

    // apply any actions we have.
    std::list<PActionF*>::iterator pfit;
    for (pfit = actions_.begin(); pfit != actions_.end(); pfit++)
    {
        (**pfit)(particles_, dt);
    }

    for (size_t i = 0; i < particles_.size(); )
    {
        Particle *part = particles_[i];
        if (part->t < 0)
        {
            delete part;
            // Use swap trick to quickly remove element
            std::swap(particles_[i], particles_.back());
            particles_.pop_back();
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
    for (size_t i = 0; i < particles_.size(); i++)
        delete particles_[i];
    particles_.clear();
}
