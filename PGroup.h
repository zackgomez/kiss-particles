// Particle Group declaration
#include <list>
#include "Particle.h"

class PGroup 
{
    void render();
    void reset();
    void update(float);
    
private:
    std::list<Particle*> particles_;

    friend class ParticleManager;
};
