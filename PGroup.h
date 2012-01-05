// Particle Group declaration
#include <list>
#include "Particle.h"
#include "PAction.h"

class PGroup 
{
public:
    void render();
    void reset();

    // Begins update, does not block
    void startUpdate(float);
    // blocks until update finishes
    void update();

    void addAction(PActionF*);
    
    int numParticles(void);
private:
    std::list<Particle*> particles_;
    std::list<PActionF*> actions_;

    float update_dt_;

    friend class ParticleManager;
};
