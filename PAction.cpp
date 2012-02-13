#include "PAction.h"
#include <iostream>
#include "utils.h"
#include <cstdio>
#include "PEmitter.h"

ConstForceF::ConstForceF(float g, const glm::vec3 &dir) :
    g_(g), dir_(glm::normalize(dir))
{ }

void ConstForceF::operator() (Particle *part, float dt)
{
    part->vel += dir_ * g_ * dt;
}

/*
PERandomF::PERandomF(float howRandom) : sigma_(howRandom) { }

void PERandomF::operator() (Emitter* em, float dt)
{
    glm::vec3 dir(randomFloat(-1, 1), randomFloat(-1, 1), randomFloat(-1, 1));
    dir = glm::normalize(dir);

    em->loc_ += dir * dt * sigma_; 

}
void DefaultActionF::operator()(std::vector<Particle*>&, float dt)
{
    return;
}

CentripetalForceF::CentripetalForceF(const glm::vec3 &center, const glm::vec3 &up,
        float radius) :
    center_(center),
    up_(glm::normalize(up)),
    radius_(radius)
{ }

void CentripetalForceF::operator() (std::vector<Particle*> &parts, float dt)
{
    for (size_t i = 0; i < parts.size(); i++)
    {
        Particle *p = parts[i];

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
}

void CentripetalForceF::setCenter(const glm::vec3 &center)
{
    center_ = center;
}

PPointAttractorF::PPointAttractorF(const glm::vec3 &pos, float magnitude) :
    pos_(pos), g_(magnitude)
{ }


void PPointAttractorF::operator() (std::vector<Particle*> &parts, float dt)
{
    for (size_t i = 0; i < parts.size(); i++)
    {
        Particle *part = parts[i];
        float r = glm::length(part->loc - pos_);
        glm::vec3 rhat = glm::normalize(part->loc - pos_);
        glm::vec3 theforce = rhat * glm::vec3(g_ / (pow(r, 2.0f))); 
        part->vel -= theforce * dt; 
    } 
}

PPointSinkF::PPointSinkF(const glm::vec3 &location, float tolerance) :
    pos_(location), tol_(tolerance)
{ }

void PPointSinkF::operator() (std::vector<Particle*> &parts, float dt)
{
    for (size_t i = 0; i < parts.size(); i++)
    {
        Particle *part = parts[i];
        if (glm::length(part->loc - pos_) < tol_)
        {
            part->t = -HUGE_VAL;
        }
    }
}


PPlaneSinkF::PPlaneSinkF(const glm::vec3 &pt, const glm::vec3 &normal) : pt_(pt), normal_(normal) { }
void PPlaneSinkF::operator() (std::vector<Particle*> &parts, float dt)
{
    for (size_t i = 0; i < parts.size(); i++)
    {
        Particle *p = parts[i];
        // check to see if it's on the same side as the plane
        if (glm::dot(normal_, p->loc - pt_) < 0.f)
            p->t = -HUGE_VAL;
    }
}

PPlaneBounceF::PPlaneBounceF(const glm::vec3 & pt, 
                  const glm::vec3 &normal_vec, 
                  float elast) :
    point_(pt), normal_(normal_vec), elasticity_(elast) { }


void PPlaneBounceF::operator() (std::vector<Particle*> &parts, float dt)
{
    // for each particle, check if it's over our plane
    for (size_t i = 0; i < parts.size(); i++)
    {
        Particle *p = parts[i];
        if (glm::dot(p->loc - point_, normal_) < 0.0f)
        {
            // particle in question is 'behind' the normal one
            reflectParticleVelocity(p);
        }
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

*/
