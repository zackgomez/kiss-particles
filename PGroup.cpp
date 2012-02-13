#include "PGroup.h"
#include <iostream>
#include <algorithm>
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

    std::vector<int> *deadPartInds;
    Mutex *dpilock;
};

void *threadfunc(void *arg)
{
    threadarg *targ = (threadarg *) arg;
    std::vector<Particle*> &particles = *(targ->particles);
    std::list<PActionF*> &actions = *(targ->actions);
    std::vector<int> &deadPartInds = *(targ->deadPartInds);
    Mutex &dpilock = *(targ->dpilock);
    float dt = targ->dt;
    int start = targ->start;
    int end = targ->end;

    // Update/integrate each particle
    for (int i = start; i < end; i++)
    {
        Particle *part = particles[i];
        if (part->t > 0)
        {
            // Update each particle first for all actions
            std::list<PActionF*>::iterator pfit;
            for (pfit = actions.begin(); pfit != actions.end(); pfit++)
                (**pfit)(part, dt);

            // Integrate
            part->update(dt);
        }
        else
        {
            // The particle is dead, add it to the list, be safe though
            dpilock.lock();
            deadPartInds.push_back(i);
            dpilock.unlock();
        }
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

    // index of dead particles
    std::vector<int> deadPartInds;

    int num_threads = 4;
    std::vector<Thread> threads_(num_threads);
    Mutex dpilock;
    for (int i = 0; i < num_threads; i++)
    {
        threadarg *arg = new threadarg();
        arg->actions = &actions_;
        arg->dt = dt;

        arg->particles = &particles_;
        arg->start = i * (particles_.size() / num_threads);
        arg->end = (i+1) * (particles_.size() / num_threads);

        arg->deadPartInds = &deadPartInds;
        arg->dpilock = &dpilock;

        threads_[i].run(threadfunc, arg);
    }

    for (size_t i = 0; i < threads_.size(); i++)
    {
        threads_[i].join();
    }

    // sort in descending order
    std::sort(deadPartInds.rbegin(), deadPartInds.rend());
    // Remove dead particles
    for (size_t i = 0; i < deadPartInds.size(); i++)
    {
        int pind = deadPartInds[i];
        delete particles_[pind];
        // Swap trick to quickly remove particles from a vector
        std::swap(particles_[pind], particles_.back());
        particles_.pop_back();
    }
}
    

void PGroup::reset(void)
{
    for (size_t i = 0; i < particles_.size(); i++)
        delete particles_[i];
    particles_.clear();
}
