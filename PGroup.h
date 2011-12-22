// Particle Group declaration
#include <list>
#include "Particle.h"
#include "PAction.h"

class PGroup 
{
public:
    void render();
    void reset();
    void update(float);
    void addAction(PActionF*);
    
private:
    std::list<Particle*> particles_;
    std::list<PActionF*> actions_;

    friend class ParticleManager;
};
