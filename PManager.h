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

    static PGroup* newGroup(const std::string &groupName);

    int numParticles(void);

private:
    ParticleManager();

    std::map<std::string, PGroup*> groups_;
    std::list<Emitter*> emitters_;

    float update_dt_;

};

