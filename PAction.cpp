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
