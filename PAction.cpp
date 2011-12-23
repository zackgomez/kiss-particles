#include "PAction.h"
#include <iostream>
#include "utils.h"

void DefaultActionF::operator()(std::list<Particle*>&, float dt)
{
    return;
}

GravityActionF::GravityActionF(float g) :
    g_(g)
{ }

void GravityActionF::operator() (std::list<Particle*> &parts, float dt)
{
    std::list<Particle*>::iterator pit;
    for (pit = parts.begin(); pit != parts.end(); pit++)
    {
        (*pit)->vel.y -= g_ * dt;
    } 

}

TornadoActionF::TornadoActionF(const glm::vec3 &center, const glm::vec3 &up) :
    center_(center),
    up_(glm::normalize(up))
{ }

void TornadoActionF::operator() (std::list<Particle*> &parts, float dt)
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

        /*
        std::cout << "pvec: " << pdir.x << ' ' << pdir.y << ' ' << pdir.z << '\n';
        std::cout << "outvec: " << outvec.x << ' ' << outvec.y << ' ' << outvec.z << '\n';
        std::cout << '\n';
        */


        // Speed of the circle component
        float speed = glm::length(sideVel);
        float radius = glm::length(outvec);
        glm::vec3 outdir = outvec / radius;

        // Apply inward force
        // inward force, centripetal force see:
        // http://en.wikipedia.org/wiki/Centripetal_force
        p->vel += -outdir * speed * speed / radius * dt * 0.6f;

        const float kUpForce = normalRandom(1.0, 0.2) * 10.f;
        const float max_upmag = 5.f;
        // Apply upward force, only if less than max
        if (glm::length(upVel) <= max_upmag)
            p->vel += up_ * kUpForce * dt;

    } 
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


PYPlaneSinkF::PYPlaneSinkF(float y_input) : y_(y_input) { }
void PYPlaneSinkF::operator() (std::list<Particle*> &parts, float dt)
{
    std::list<Particle*>::iterator pit;
    for (pit = parts.begin(); pit != parts.end(); pit++)
    {
        if ((*pit)->loc.y < y_)
        {
            delete *pit;
            pit = parts.erase(pit);
        }
    }
}
