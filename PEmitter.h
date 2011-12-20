#pragma once
#include "Particle.h"
#include <list>
#include <glm/glm.hpp>
#include <vector>

float normalRandom(float mu, float sigma);

// Forward declaration necessary for friending
class ParticleManager;

////////////////////////////////////////////////////
// Functors
/////////////////////////////////////////////////////

// The generic, simple lifetime function
struct lifetimeF
{
    lifetimeF(float mu) : mu_(mu) { }
    virtual float operator () () { return mu_; }

protected:
    float mu_;
};

struct lifetimeNormalF : lifetimeF
{
    lifetimeNormalF(float mu, float variance);
    virtual float operator () ();
private:
    float var_;
};

// Simpleton velocity function
struct velocityF
{
    velocityF(float radius, float mu, float sigma) : mu_(mu), sigma_(sigma), r_(radius) { }
    virtual glm::vec3 operator() (const glm::vec3 &epos, const glm::vec3 &ppos);

protected:
    float mu_, sigma_;
    float r_;
};

struct locationF
{
    locationF(float radius) : r_(radius) { }

    virtual glm::vec3 operator()(const glm::vec3 &epos);
protected:
    float r_;
};

struct colorF
{
    colorF() { }
    colorF(const glm::vec4 &color, float bright, float brightvar) :
        mu_(color), bright_(bright), brightvar_(brightvar)
    {
    }
    virtual ~colorF() {}

    virtual glm::vec4 operator() ();

protected:
    glm::vec4 mu_;
    float bright_, brightvar_;
};

struct discreteColorF : public colorF
{
    discreteColorF(const std::vector<glm::vec4> &colors) : colors_(colors)
    { }

    virtual glm::vec4 operator() ();
protected:
    std::vector<glm::vec4> colors_;
};

//
// The emitter class is the main particle engine configuration point.
// With its setter methods particle lifetime, location, and spew rate
// can be defined.
// Emitters will spew particles (and take up clock cycles) until a call to
// ParticleManager::quashEmitter()
//
class Emitter 
{
public:
    Emitter* setParticleLocationF(locationF *);
    Emitter* setParticleLifetimeF(lifetimeF *);
    Emitter* setParticleVelocityF(velocityF *);
    Emitter* setParticleColorF(colorF *);

    Emitter* setLocation(const glm::vec3 &l);
    Emitter* setOutputRate(float r);
    Emitter* setRadius(float r);
    // How much time is left before this emitter expires?
    Emitter* setTimeRemaining(float);

    bool isDone() const;
    // The update function. Spew some new particles, given that dt seconds
    // have elapsed.
    void emit(std::list<Particle*>&, float dt);
private:
    Emitter();

    // Lifetime, and a functor that determines particle lifetime
    lifetimeF *lifetime_func;
    // particle velocity, and its associated functor
    velocityF *velocity_func;
    locationF *location_func;
    colorF *color_func;

    // Since this is a sperical emitter, we need two pieces of information:
    glm::vec3 loc_;
    // Particles will spawn with this initial velocity
    // And with a frequency determined by this (particles per second)
    float rate_;
    // Each particle will be about this size.
    glm::vec3 size_;

    float timeRemaining_;

    friend class ParticleManager;
};


