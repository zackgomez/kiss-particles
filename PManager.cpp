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

Particle * ParticleManager::newParticle(const std::string &groupName)
{
    if (groups_.find(groupName) == groups_.end())
    {
        std::cerr << "FATAL ERROR: unable to find Particle Group `" << groupName << "`\n";
        assert(false);
        return NULL;
    }

    return groups_[groupName]->newParticle();
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

    // Time the creation step
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int creation_start = tv.tv_usec;
    
    // First create new particles.
    std::list<Emitter*>::iterator eit;
    for (eit = emitters_.begin(); eit != emitters_.end(); )
    {
        std::string og = (*eit)->getOutputGroup();
        // Exit if we can't find the specified output group
        if (!groups_.count(og)) 
        {
            std::cout << "FATAL ERROR: unknown particle group requested: " << og << std::endl; 
            assert(false);
        }
        // Emit into the output particle group
        (*eit)->emit(dt);
        if ((*eit)->isDone()) 
        {
            delete *eit;
            eit = emitters_.erase(eit);
        }
		else
			eit++;
    }

    // Record creation time
    gettimeofday(&tv, NULL);
    int creation_usec = tv.tv_usec - creation_start;
    // be careful of wrap
    creation_usec = creation_usec >= 0 ? creation_usec : 1e6 + creation_usec;
    if (recording_)
        times_[0] += creation_usec;

    // Start timing update
    updateStart_ = tv.tv_usec;
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

    // Finish the particle update (blocks until each group is completed)
    std::map<std::string, PGroup*>::iterator pit;
    for (pit = groups_.begin(); pit != groups_.end(); pit++) 
    {
        pit->second->update();
    }

    // Finish timing
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int update_usec = tv.tv_usec - updateStart_;
    update_usec = update_usec >= 0 ? update_usec : 1e6 + update_usec;

    if (recording_)
    {
        times_[1] += update_usec;
        frame_count_++;
    }
}

    

void ParticleManager::render(float dt)
{
    // Time the render
    struct timeval tv;
    gettimeofday(&tv, NULL);
    suseconds_t start_usec = tv.tv_usec;

    // draw existing particles
    std::map<std::string, PGroup*>::iterator pit;
    for (pit = groups_.begin(); pit != groups_.end(); pit++) 
    {
        pit->second->render();
    }

    // Finish the timing
    gettimeofday(&tv, NULL);
    suseconds_t render_usec = tv.tv_usec - start_usec;
    render_usec = render_usec >= 0 ? render_usec : 1e6 + render_usec;
    if (recording_)
        times_[2] += render_usec;
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
    //std::cout << "Adding a new emitter: " << em << std::endl;
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

    // Remove each pgroup and readd default
    std::map<std::string, PGroup*>::iterator git;
    for (git = groups_.begin(); git != groups_.end(); git++)
        delete git->second;
    groups_.clear();

    groups_["default"] = new PGroup();   
}

#ifdef KISS_PARTICLES_DEBUG
void ParticleManager::startRecording()
{
    recording_= true;
    frame_count_ = 0;
    times_[0] = 0.f;
    times_[1] = 0.f;
    times_[2] = 0.f;
}

const double* ParticleManager::stopRecording()
{
    recording_ = false;

    times_[0] /= frame_count_;
    times_[1] /= frame_count_;
    times_[2] /= frame_count_;

    return times_;
}
#endif
