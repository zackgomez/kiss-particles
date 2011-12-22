#include <iostream>
#include "PManager.h"
#include "Particle.h"
#include "PEmitter.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cstdlib>

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
    

// A combined update/render call...
// Two types of operations occur here:
//  - those that update emitters and particles
//  - those that render particles
void ParticleManager::render(float dt)
{
    /*
    std::cout << "Rendering " << emitters_.size() << " emitters and "
        << groups_.size() << " groups\n";
        */
    // First create new particles.
    std::list<Emitter*>::iterator eit;
    for (eit = emitters_.begin(); eit != emitters_.end(); eit++)
    {
        std::string og = (*eit)->outputGroup;
        //DPRINT(groups_.find(og)->second);
        // Freak the fuck out if we can't find the requested group.
        if (!groups_.count(og)) 
        {
            std::cout << "unknown particle group requested: " << og << std::endl; 
            assert(false);
        }
        (*eit)->emit(groups_.find(og)->second->particles_, dt);
        if ((*eit)->isDone()) 
        {
            delete *eit;
            eit = emitters_.erase(eit);
        }
    }
 
    // finally draw existing particles
    std::map<std::string, PGroup*>::iterator pit;
    for (pit = groups_.begin(); pit != groups_.end(); pit++) 
    {
        //std::cout << "attempting to render group:" << pit->second << std::endl;
        pit->second->update(dt);
        pit->second->render();
    }
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
