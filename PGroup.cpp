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
    std::list<PActionF*>   *actions;
    float dt;

    std::vector<Particle*> *particles;
    int start, end;
};

void *threadfunc(void *arg)
{
    threadarg *targ = (threadarg *) arg;
    std::vector<Particle*> &particles = *(targ->particles);
    std::list<PActionF*> &actions = *(targ->actions);
    float dt = targ->dt;
    int start = targ->start;
    int end = targ->end;

    // Update/integrate each particle
    for (int i = start; i < end; i++)
    {
        // Update each particle first for all actions
        std::list<PActionF*>::iterator pfit;
        for (pfit = actions.begin(); pfit != actions.end(); pfit++)
            (**pfit)(particles[i], dt);

        // Integrate
        particles[i]->update(dt);
    }
    

    // Clean up and finish
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

    int num_threads = 1;
    std::vector<Thread> threads_(num_threads);
    for (int i = 0; i < num_threads; i++)
    {
        threadarg *arg = new threadarg();
        arg->actions = &actions_;
        arg->particles = &particles_;
        arg->dt = dt;
        arg->start = i * (particles_.size() / num_threads);
        arg->end = (i+1) * (particles_.size() / num_threads);

        threads_[i].run(threadfunc, arg);
    }

    for (size_t i = 0; i < threads_.size(); i++)
    {
        threads_[i].join();
    }

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
