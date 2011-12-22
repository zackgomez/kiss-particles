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
