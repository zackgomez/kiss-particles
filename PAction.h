#pragma once
#include <list>
#include "Particle.h"
#include <glm/glm.hpp>

class PActionF
{
public:
    virtual void operator() (std::list<Particle*> &, float dt) = 0;
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

class TornadoActionF : public PActionF
{
public:
    TornadoActionF(const glm::vec3 &center, const glm::vec3 &up);
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

class PYPlaneSinkF : public PActionF
{
public:
    PYPlaneSinkF(float y);
    virtual void operator() (std::list<Particle*> &, float);
private:
    float y_;
};
