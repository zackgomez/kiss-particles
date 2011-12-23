#include "PAction.h"
#include <iostream>
#include "utils.h"
#include "PEmitter.h"

PERandomF::PERandomF(float howRandom) : sigma_(howRandom) { }

void PERandomF::operator() (Emitter* em, float dt)
{
    glm::vec3 dir(randomFloat(-1, 1), randomFloat(-1, 1), randomFloat(-1, 1));
    dir = glm::normalize(dir);

    em->loc_ += dir * dt * sigma_; 

}
void DefaultActionF::operator()(std::list<Particle*>&, float dt)
{
    return;
}

ConstForceF::ConstForceF(float g, const glm::vec3 &dir) :
    g_(g), dir_(glm::normalize(dir))
{ }

void ConstForceF::operator() (std::list<Particle*> &parts, float dt)
{
    std::list<Particle*>::iterator pit;
    for (pit = parts.begin(); pit != parts.end(); pit++)
    {
        (*pit)->vel += dir_ * g_ * dt;
    } 

}

CentripetalForceF::CentripetalForceF(const glm::vec3 &center, const glm::vec3 &up) :
    center_(center),
    up_(glm::normalize(up))
{ }

void CentripetalForceF::operator() (std::list<Particle*> &parts, float dt)
{
    std::list<Particle*>::iterator pit;
    for (pit = parts.begin(); pit != parts.end(); pit++)
    {
        Particle *p = *pit;

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
        float radius = glm::length(outvec);
        glm::vec3 outdir = outvec / radius;

        // Apply inward force
        // inward force, centripetal force see:
        // http://en.wikipedia.org/wiki/Centripetal_force
        p->vel += -outdir * speed * speed / radius * dt;
    } 
}

void CentripetalForceF::setCenter(const glm::vec3 &center)
{
    center_ = center;
}

PPointAttractorF::PPointAttractorF(const glm::vec3 &pos, float magnitude) :
    pos_(pos), g_(magnitude)
{ }


void PPointAttractorF::operator() (std::list<Particle*> &parts, float dt)
{

    std::list<Particle*>::iterator pit;
    for (pit = parts.begin(); pit != parts.end(); pit++)
    {
        float r = glm::length((*pit)->loc - pos_);
        glm::vec3 rhat = glm::normalize((*pit)->loc - pos_);
        glm::vec3 theforce = rhat * glm::vec3(g_ / (pow(r, 2.0f))); 
       (*pit)->vel -= theforce * dt; 
    } 
}

PPointSinkF::PPointSinkF(const glm::vec3 &location, float tolerance) :
    pos_(location), tol_(tolerance)
{ }

void PPointSinkF::operator() (std::list<Particle*> &parts, float dt)
{
    std::list<Particle*>::iterator pit;
    for (pit = parts.begin(); pit != parts.end(); pit++)
    {
        if ( glm::length((*pit)->loc - pos_) < tol_)
        {
            delete *pit;
            pit = parts.erase(pit);
        }
    }
}


PPlaneSinkF::PPlaneSinkF(const glm::vec3 &pt, const glm::vec3 &normal) : pt_(pt), normal_(normal) { }
void PPlaneSinkF::operator() (std::list<Particle*> &parts, float dt)
{
    std::list<Particle*>::iterator pit;
    for (pit = parts.begin(); pit != parts.end(); pit++)
    {
        Particle *p = *pit;
        // check to see if it's on the same side as the plane
        if (glm::dot(normal_, p->loc - pt_) < 0.f)
        {
            delete *pit;
            pit = parts.erase(pit);
        }
    }
}
