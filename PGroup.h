// Particle Group declaration
#include <list>
#include "Particle.h"
#include "PAction.h"

class PGroup 
{
public:
    PGroup();
    ~PGroup();

    void render();
    void reset();

    // Begins update, does not block
    void startUpdate(float);
    // blocks until update finishes
    void update();

    Particle *newParticle();

    void setAction(PActionF*);
    
    int numParticles();
private:
    std::vector<Particle> particles_;
    PActionF* action_;

    float update_dt_;
};

void renderParticles(const std::vector<Particle> &data);
