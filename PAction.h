#pragma once
#include <list>
#include "Particle.h"


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

class PPointAttractorF : public PActionF
{
public:
    PPointAttractorF(const glm::vec3 &pos, float magnitude);
    virtual void operator() (std::list<Particle*> &, float);
private:
    glm::vec3 pos_;
    float g_;
};

