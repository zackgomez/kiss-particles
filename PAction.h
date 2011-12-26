#pragma once
#include <list>
#include "Particle.h"
#include <glm/glm.hpp>

class Emitter;
 
class PActionF
{
public:
    virtual void operator() (std::list<Particle*> &, float dt) = 0;
};

class PEmitterActionF
{
public:
    virtual void operator() (Emitter*, float) = 0;
};

class PERandomF : virtual public PEmitterActionF
{
public:
    PERandomF(float howRandom);
    virtual void operator() (Emitter*, float);
private:
    float sigma_;
};

class DefaultActionF : public PActionF 
{
public:
    virtual void operator()(std::list<Particle*>&, float dt);
};

class ConstForceF : public PActionF
{
public:
    // Constructor takes the gravity constant
    ConstForceF(float g, const glm::vec3 &dir);
    virtual void operator() (std::list<Particle*> &, float dt);
private:
    float g_;
    const glm::vec3 dir_;
};

// Centripetal force around a line oriented by center and up
class CentripetalForceF : public PActionF
{
public:
    CentripetalForceF(const glm::vec3 &center, const glm::vec3 &up,
            float radius);
    virtual void operator() (std::list<Particle*> &, float dt);

    void setCenter(const glm::vec3& center);

private:
    glm::vec3 center_;
    const glm::vec3 up_;
    const float radius_;
};

class PPointAttractorF : public PActionF
{
public:
    PPointAttractorF(const glm::vec3 &pos, float magnitude);
    virtual void operator() (std::list<Particle*> &, float);
private:
    glm::vec3 pos_;
    float g_;
};

class PPointSinkF : public PActionF
{
public:
    PPointSinkF(const glm::vec3 &, float tolerance);
    virtual void operator() (std::list<Particle*> &, float);
private:
    glm::vec3 pos_;
    float tol_;
};

class PPlaneSinkF : public PActionF
{
public:
    PPlaneSinkF(const glm::vec3 &pt, const glm::vec3 &normal);
    virtual void operator() (std::list<Particle*> &, float);

private:
    glm::vec3 pt_, normal_;
};

// Bounce action. For example, sparks bouncing off the floor.
// Allows for non-elastic collisions, so particles don't bounce indefinitely.
// (See the SC2 sparks on Terran command centers on Ultra graphics)
class PPlaneBounceF : public PActionF
{
public:
    // Constructor needs to specify a planepoint, normal vector, and elasticity 
    PPlaneBounceF(const glm::vec3 & pt, 
                  const glm::vec3 &normal_vec, 
                  float elasticity);

    virtual void operator() (std::list<Particle*> &, float);

    void reflectParticleVelocity(Particle &p);

private:
    // vector and a point needed to specify a plane.
    glm::vec3 point_;
    glm::vec3 normal_;
    float elasticity_;
};

