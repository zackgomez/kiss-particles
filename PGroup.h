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

    void addAction(PActionF*);
    
    int numParticles();
private:
    // Front and back AKA read and write
    std::vector<Particle*> particles_[2];
    int partInd_;
    std::list<PActionF*> actions_;

    float update_dt_;
};


struct particleData
{
    glm::vec3 pos;
    glm::vec4 color;
};
 
void renderParticles(const std::vector<particleData> &data);
