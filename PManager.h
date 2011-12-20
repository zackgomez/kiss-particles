#pragma once

#include <glm/glm.hpp>
#include <list>
#include "Particle.h"
#include "PEmitter.h"

// Utility functions

glm::vec3 pointOnSphere(float r, glm::vec3 pos);  
float normalRandom(float mu, float sigma);

//
// Particle system manager declaration.
//
// Singleton and Factory Pattern
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

private:
    ParticleManager();
    ~ParticleManager();

    std::list<Emitter*> emitters_;
    std::list<Particle*> particles_;

};

