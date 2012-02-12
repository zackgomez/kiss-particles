#pragma once

#include <glm/glm.hpp>
#include <list>
#include <map>
#include <string>
#include "Particle.h"
#include "PEmitter.h"
#include "PGroup.h"


//
// Particle system manager declaration.
//
class ParticleManager 
{
public:
    ~ParticleManager();

    // Get() access to the singleton instance
    static ParticleManager* get();

    // kicks off an update, call early, does not block
    void startUpdate(float dt);
    // blocks until update finishes.
    void update();

    void render(float dt);
    Emitter* newEmitter();
    // 
    void addEmitter(Emitter*);
    // Get rid of this emitter.
    void quashEmitter(Emitter*);

    // Removes all emitters and particles
    void reset();

    PGroup* newGroup(const std::string &groupName);
    Particle *newParticle(const std::string &groupName);

    int numParticles(void);

#ifdef KISS_PARTICLES_DEBUG
    void startRecording();
    const double* stopRecording();
#endif

private:
    ParticleManager();

    std::map<std::string, PGroup*> groups_;
    std::list<Emitter*> emitters_;

    float update_dt_;

    bool recording_;
    // The time an update started in system usec
    int updateStart_;
    // total frames recording
    unsigned frame_count_;
    // Total times in each stage (in usec) (creation, update, render)
    double times_[3];
};

