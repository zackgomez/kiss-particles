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

class GravityActionF : public PActionF
{
public:
    // Constructor takes the gravity constant
    GravityActionF(float g);
    virtual void operator() (std::list<Particle*> &, float dt);
private:
    float g_;
};

// Centripetal force around a line oriented by center and up
class CentripetalForceF : public PActionF
{
public:
    CentripetalForceF(const glm::vec3 &center, const glm::vec3 &up);
    virtual void operator() (std::list<Particle*> &, float dt);

private:
    const glm::vec3 center_;
    const glm::vec3 up_;
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
