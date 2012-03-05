#pragma once
#include "Particle.h"
#include <list>
#include <glm/glm.hpp>
#include <vector>
#include <string>
 
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
    velocityF() : mu_(0), sigma_(0), vel_(0) { }
    velocityF(float speed, float mu, float sigma) : mu_(mu), sigma_(sigma), vel_(speed) { }
    virtual glm::vec3 operator() (const glm::vec3 &epos, const glm::vec3 &ppos);

protected:
    float mu_, sigma_;
    float vel_;
};

struct velocityCombinerF : public velocityF
{
    // combines f1 with f2 like a*f1 + (1-a)*f2
    velocityCombinerF(velocityF *f1, velocityF *f2, float a) :
        f1_(f1), f2_(f2), a_(a)
    { }
    ~velocityCombinerF();

    virtual glm::vec3 operator() (const glm::vec3 &epos, const glm::vec3 &ppos);

protected:
    velocityF *f1_, *f2_;
    const float a_;
};

struct velocityAdderF : public velocityF
{
    velocityAdderF(const glm::vec2 *vel, velocityF *f);
    ~velocityAdderF();
    virtual glm::vec3 operator() (const glm::vec3 &epos, const glm::vec3 &ppos);

protected:
    const glm::vec2 *vel_;
    velocityF *f_;
};

// Assumes that locations are on a circle around epos. Gives them a velocity
// pointing "up" and away from the center
struct coneVelocityF : public velocityF
{
    // upfact is in [0, 1] and determines amount of "up velocity"
    coneVelocityF(float mu, float sigma, const glm::vec3 &up, float upfact) :
        mu_(mu), sigma_(sigma), upvec_(glm::normalize(up)), upfact_(upfact)
    { }

    virtual glm::vec3 operator()(const glm::vec3 &epos, const glm::vec3 &ppos);

protected:
    // hiding velocityF members because it should be just an interface
    float mu_, sigma_;
    glm::vec3 upvec_;
    float upfact_;
};

struct circleTangentVelocityF : public velocityF
{
    circleTangentVelocityF(float mu, float sigma, const glm::vec3 &upvec) :
        mu_(mu), sigma_(sigma), upvec_(upvec)
    { }

    virtual glm::vec3 operator()(const glm::vec3 &epos, const glm::vec3 &ppos);

protected:
    const float mu_, sigma_;
    const glm::vec3 upvec_;
};

struct locationF
{
    locationF() : r_(0.f) { }
    locationF(float radius) : r_(radius) { }
    virtual ~locationF() { }

    virtual glm::vec3 operator()(const glm::vec3 &epos);
protected:
    float r_;
};

// Creates particles on a circle, with radius and orientation
struct circleLocationF : public locationF
{
    circleLocationF(float radius, const glm::vec3 &up) :
        r_(radius), upvec_(glm::normalize(up))
    { }

    virtual glm::vec3 operator()(const glm::vec3 &epos);

protected:
    // Repeated because eventually locationF will be just an interface
    float r_;
    glm::vec3 upvec_;
};

struct sphereInteriorLocationF : public locationF
{
    sphereInteriorLocationF(float radius) : r_(radius) { }

    virtual glm::vec3 operator()(const glm::vec3 &epos);
protected:
    float r_;
};

// Creates particles on the interior a circle, with radius and orientation
struct circleInteriorLocationF : public circleLocationF
{
    circleInteriorLocationF(float radius, const glm::vec3 &up);
    virtual glm::vec3 operator()(const glm::vec3 &epos);

protected:
    // Repeated because eventually locationF will be just an interface
    float r_;
    glm::vec3 upvec_;
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

class PEmitterActionF;

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
    Emitter* setParticleSize(const glm::vec3 &size);

    Emitter* setLocation(const glm::vec3 &l);
    Emitter* setOutputRate(float r);
    // How much time is left before this emitter expires?
    Emitter* setTimeRemaining(float);

    Emitter* setOutputGroup(const std::string &);
    std::string getOutputGroup() const;

    bool isDone() const;
    // The update function. Spew some new particles, given that dt seconds
    // have elapsed.
    void emit(float dt);

    void addEmitterAction(PEmitterActionF *);


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

    std::list<PEmitterActionF *> eactions_;
    std::string outputGroup_;   
    float timeRemaining_;
    float particlesToEmit_;


    friend class PERandomF;
    friend class ParticleManager;
};

