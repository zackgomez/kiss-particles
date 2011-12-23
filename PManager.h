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
    // Get() access to the singleton instance
    static ParticleManager* get();

    void render(float dt);
    Emitter* newEmitter();
    // 
    void addEmitter(Emitter*);
    // Get rid of this emitter.
    void quashEmitter(Emitter*);

    // Removes all emitters and particles
    void reset();

    static PGroup* newGroup(const std::string &groupName);

private:
    ParticleManager();
    ~ParticleManager();

    std::map<std::string, PGroup*> groups_;
    std::list<Emitter*> emitters_;

};

