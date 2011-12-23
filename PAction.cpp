#include "PAction.h"

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

