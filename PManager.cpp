#include <iostream>
#include "PManager.h"
#include "Particle.h"
#include "PEmitter.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cstdlib>
#ifdef KISS_PARTICLES_DEBUG
#include <sys/time.h>
#endif
 
ParticleManager::ParticleManager()
{
    // Make sure to set up the default particle group.
    groups_["default"] = new PGroup();   
}

ParticleManager::~ParticleManager()
{
    reset();
}

PGroup* ParticleManager::newGroup(const std::string &gname)
{
    PGroup *p = new PGroup();
    get()->groups_[gname] = p;
    return p;
}

int ParticleManager::numParticles(void)
{
    int cnt = 0;
    std::map<std::string, PGroup*>::iterator pit;
    for (pit = groups_.begin(); pit != groups_.end(); pit++)
    {
        cnt += pit->second->numParticles();
    }
    return cnt;
}

void ParticleManager::startUpdate(float dt)
{
    update_dt_ = dt;

    // Kick off each PGroup update
    std::map<std::string, PGroup*>::iterator pit;
    for (pit = groups_.begin(); pit != groups_.end(); pit++) 
    {
        pit->second->startUpdate(dt);
    }
}

void ParticleManager::update()
{
    float dt = update_dt_;
#ifdef KISS_PARTICLES_DEBUG
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int start_usec = tv.tv_usec;
#endif

    // First create new particles.
    std::list<Emitter*>::iterator eit;
    for (eit = emitters_.begin(); eit != emitters_.end(); )
    {
        std::string og = (*eit)->outputGroup_;
        //DPRINT(groups_.find(og)->second);
        // Freak the fuck out if we can't find the requested group.
        if (!groups_.count(og)) 
        {
            std::cout << "FATAL ERROR: unknown particle group requested: " << og << std::endl; 
            assert(false);
        }
        // Emit into the output particle group
        (*eit)->emit(groups_.find(og)->second->particles_, dt);
        if ((*eit)->isDone()) 
        {
            delete *eit;
            eit = emitters_.erase(eit);
        }
		else
			eit++;
    }

#ifdef KISS_PARTICLES_DEBUG
    gettimeofday(&tv, NULL);
    int creation_usec = tv.tv_usec - start_usec;
#endif
 
    // Update and move particles
    std::map<std::string, PGroup*>::iterator pit;
    for (pit = groups_.begin(); pit != groups_.end(); pit++) 
    {
        pit->second->update();
    }
#ifdef KISS_PARTICLES_DEBUG
    gettimeofday(&tv, NULL);
    int update_usec = tv.tv_usec - start_usec - creation_usec;
#endif

#ifdef KISS_PARTICLES_DEBUG
    std::cout << "ParticleManager::update() - creation: " << creation_usec
        << "us update: " << update_usec << "us\n";
#endif
}

    

// A combined update/render call...
// Two types of operations occur here:
//  - those that update emitters and particles
//  - those that render particles
void ParticleManager::render(float dt)
{
#ifdef KISS_PARTICLES_DEBUG
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int start_usec = tv.tv_usec;
#endif

    // draw existing particles
    std::map<std::string, PGroup*>::iterator pit;
    for (pit = groups_.begin(); pit != groups_.end(); pit++) 
    {
        pit->second->render();
    }
#ifdef KISS_PARTICLES_DEBUG
    gettimeofday(&tv, NULL);
    int render_usec = tv.tv_usec - start_usec;

    std::cout << "ParticleManager::render() - " << render_usec << "us\n";
#endif
}

ParticleManager* ParticleManager::get()
{
    static ParticleManager pm;
    return &pm;
}

Emitter* ParticleManager::newEmitter()
{
    return new Emitter();
}

void ParticleManager::addEmitter(Emitter *em)
{
    std::cout << "Adding a new emitter: " << em << std::endl;
    emitters_.push_back(em);
}

void ParticleManager::quashEmitter(Emitter *e)
{
    assert(e);
    delete e;
    emitters_.remove(e);
}

void ParticleManager::reset()
{
    std::list<Emitter*>::iterator eit;
    for (eit = emitters_.begin(); eit != emitters_.end(); eit++)
        delete *eit;
    emitters_.clear();

    // Tell each particle group to reset.
}
