#include "PAction.h"
#include <iostream>
#include "utils.h"
#include <cstdio>
#include "PEmitter.h"

PERandomF::PERandomF(float howRandom) : sigma_(howRandom) { }

void PERandomF::operator() (Emitter* em, float dt)
{
    glm::vec3 dir(randomFloat(-1, 1), randomFloat(-1, 1), randomFloat(-1, 1));
    dir = glm::normalize(dir);

    em->loc_ += dir * dt * sigma_; 

}
void DefaultActionF::operator() (Particle *part, float dt)
{
    return;
}

MultiActionF::~MultiActionF()
{
    for (size_t i = 0; i < actions_.size(); i++)
        delete actions_[i];
}

void MultiActionF::operator() (Particle *p, float dt)
{
    // apply each action in order
    for (size_t i = 0; i < actions_.size(); i++)
        (*actions_[i])(p, dt);
}

void MultiActionF::addAction(PActionF *pa)
{
    actions_.push_back(pa);
}

ConstForceF::ConstForceF(float g, const glm::vec3 &dir) :
    g_(g), dir_(glm::normalize(dir))
{ }

void ConstForceF::operator() (Particle *part, float dt)
{
    part->vel += dir_ * g_ * dt;
}

CentripetalForceF::CentripetalForceF(const glm::vec3 &center, const glm::vec3 &up,
        float radius) :
    center_(center),
    up_(glm::normalize(up)),
    radius_(radius)
{ }

void CentripetalForceF::operator() (Particle *p, float dt)
{
    // First we need to get the direction from the center to the point,
    // use project for this
    glm::vec3 pdir = p->loc - center_;
    // up is unit vector, no need for divison by ||up||
    glm::vec3 projection = glm::dot(pdir, up_) * up_;
    // Outward vector is pdir - projection
    glm::vec3 outvec = pdir - projection;

    // Get the velocity not in the upwards direction
    glm::vec3 upVel = glm::dot(p->vel, up_) * up_;
    glm::vec3 sideVel = p->vel - upVel;

    // Speed of the circle component
    float speed = glm::length(sideVel);
    glm::vec3 outdir = glm::normalize(outvec);

    // Apply inward force
    // inward force, centripetal force see:
    // http://en.wikipedia.org/wiki/Centripetal_force
    p->vel += -outdir * speed * speed / radius_ * dt;
} 

void CentripetalForceF::setCenter(const glm::vec3 &center)
{
    center_ = center;
}

PPointAttractorF::PPointAttractorF(const glm::vec3 &pos, float magnitude) :
    pos_(pos), g_(magnitude)
{ }


void PPointAttractorF::operator() (Particle *part, float dt)
{
    float r = glm::length(part->loc - pos_);
    glm::vec3 rhat = glm::normalize(part->loc - pos_);
    glm::vec3 theforce = rhat * glm::vec3(g_ / (pow(r, 2.0f))); 
    part->vel -= theforce * dt; 
}

PPointSinkF::PPointSinkF(const glm::vec3 &location, float tolerance) :
    pos_(location), tol_(tolerance)
{ }

void PPointSinkF::operator() (Particle *part, float dt)
{
    if (glm::length(part->loc - pos_) < tol_)
    {
        part->t = -HUGE_VAL;
    }
}


PPlaneSinkF::PPlaneSinkF(const glm::vec3 &pt, const glm::vec3 &normal) : pt_(pt), normal_(normal) { }
void PPlaneSinkF::operator() (Particle *part, float dt)
{
    // check to see if it's on the same side as the plane
    if (glm::dot(normal_, part->loc - pt_) < 0.f)
        part->t = -HUGE_VAL;
}

PPlaneBounceF::PPlaneBounceF(const glm::vec3 & pt, 
                  const glm::vec3 &normal_vec, 
                  float elast) :
    point_(pt), normal_(normal_vec), elasticity_(elast) { }


void PPlaneBounceF::operator() (Particle *part, float dt)
{
    if (glm::dot(part->loc - point_, normal_) < 0.0f)
    {
        // particle in question is 'behind' the normal one
        reflectParticleVelocity(part);
    }
}

void PPlaneBounceF::reflectParticleVelocity(Particle *p)
{
    float magnitude = glm::length(p->vel);
    //glm::vec3 dn = normal_ - glm::normalize(p.vel);
    //glm::vec3 oldvel = p.vel;
    // reflected vector

    p->vel = magnitude * glm::normalize((normal_ - glm::normalize(p->vel)));
    p->vel = elasticity_ * magnitude *  glm::reflect(-glm::normalize(p->vel), normal_);
}
