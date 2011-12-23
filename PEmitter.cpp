#define _USE_MATH_DEFINES
#include "PEmitter.h"
#include <cstdio>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "utils.h"
#include "PAction.h"

lifetimeNormalF::lifetimeNormalF(float mu_, float variance) 
    : lifetimeF(mu_), var_(variance)
{ }

float lifetimeNormalF::operator () ()
{
    return normalRandom(mu_, var_); 
}

glm::vec3 velocityF::operator() (const glm::vec3 &epos, const glm::vec3 &ppos)
{
    return normalRandom(mu_, 0) * glm::normalize(epos - ppos) / vel_;
}

velocityCombinerF::~velocityCombinerF()
{
    delete f1_;
    delete f2_;
}

glm::vec3 velocityCombinerF::operator() (const glm::vec3 &epos, const glm::vec3 &ppos)
{
    return a_ * (*f1_)(epos, ppos) + (1 - a_) * (*f2_)(epos, ppos);
}

glm::vec3 coneVelocityF::operator() (const glm::vec3 &epos, const glm::vec3 &ppos)
{
    glm::vec3 circledir = glm::normalize(epos - ppos);
    // Point in upvec direction according to upfact
    circledir = glm::normalize((1 - upfact_) * circledir + upfact_ * upvec_);

    float velmag = normalRandom(mu_, sigma_);

    return circledir * velmag;
}

glm::vec3 circleTangentVelocityF::operator() (const glm::vec3 &epos, const glm::vec3 &ppos)
{
    glm::vec3 circledir = glm::normalize(epos - ppos);
    glm::vec3 tandir = glm::cross(upvec_, circledir);

    float velmag = normalRandom(mu_, sigma_);
    return velmag * tandir;
}

glm::vec3 locationF::operator() (const glm::vec3 &epos)
{
    return pointOnSphere(r_, epos);
}

glm::vec3 circleLocationF::operator() (const glm::vec3 &epos)
{
    // Use info at http://www.math4all.in/public_html/linear%20algebra/chapter8.2.html
    // to construct orthonormal basis from up vector.  Then given these extra
    // two orthogonal vectors rotate one of them a random angle around the up vector
    // and that will give the direction, multiply by the radius and add to center
    
    // In homogenous coords
    glm::vec4 orthvec = glm::vec4(upvec_.y,
            -1 + upvec_.y * upvec_.y * 1 / (1 + upvec_.x),
            0  + upvec_.y * upvec_.z * 1 / (1 + upvec_.x),
            1);

    float angle_deg = randomFloat(0, 360);

    glm::mat4 rotmat = glm::rotate(glm::mat4(1.f), angle_deg,
            upvec_);

    glm::vec4 homodir = rotmat * orthvec;
    homodir /= homodir.w;
    glm::vec3 dir(homodir);

    return epos + dir * r_;
}

glm::vec4 colorF::operator() ()
{
    return mu_ * normalRandom(bright_, brightvar_);
}

glm::vec4 discreteColorF::operator() ()
{
    int ind = rand() % colors_.size();
    return colors_[ind];
}

Emitter::Emitter() :
    outputGroup("default"),
    lifetime_func(new lifetimeF(0.3f)),
    velocity_func(new velocityF(2.f, 50.f, 10.f)),
    location_func(new locationF(10.f)),
    color_func(new colorF(glm::vec4(1.f, 1.f, 1.f, 0.f), 0.8, 0.2)),
    loc_(glm::vec3(0.f)),
    rate_(400.f),
    size_(1.f),
    timeRemaining_(HUGE_VAL)
{
}

Emitter* Emitter::setTimeRemaining(float r)
{
    timeRemaining_ = r;
    return this;
}

bool Emitter::isDone() const 
{
    return timeRemaining_ < 0;
}


void Emitter::addEmitterAction(PEmitterActionF *eaf)
{
    eactions_.push_back(eaf);
}

void Emitter::emit(std::list<Particle*>& particles, float dt) 
{

    // first apply emitter actions
    std::list<PEmitterActionF*>::iterator ait;
    for (ait = eactions_.begin(); ait != eactions_.end(); ait++)
    {
        (**ait)(this, dt);
    }

    // first decrease time remaining _on the emitter_.
    timeRemaining_ -= dt;

    int numNewParts = rate_ * dt;
    for (int j = 0; j < numNewParts; j++)     
    {
        Particle *p = new Particle();
        p->loc = (*location_func)(loc_);
        p->t = (*lifetime_func)();
        // Particle's velocity is the normal at that point
        // scaled by emitter's velocity value and given some
        // random nonsense
        p->vel = (*velocity_func)(p->loc, loc_);
        p->size = glm::vec3(size_);
        p->color = (*color_func)();

        /*
        glm::vec4 colordelta = glm::vec4(
                normalRandom(1.f, colorvar_.r),
                normalRandom(1.f, colorvar_.g),
                normalRandom(1.f, colorvar_.b),
                1.f);
        colordelta = glm::max(colordelta, 0.f);
        p->color = glm::clamp(color_ * colordelta, 0.f, 1.f);
        p->color *= normalRandom(colorbright_, colorbrightvar_);
        */
        particles.push_back(p);
    }
}

Emitter* Emitter::setParticleLifetimeF(lifetimeF *lf)
{
    if (lifetime_func)
    {
        delete lifetime_func;
    }
    lifetime_func = lf;
    return this;
}

Emitter* Emitter::setParticleVelocityF(velocityF *vf)
{
    if (velocity_func)
    {
        delete velocity_func;
    }
    velocity_func = vf;
    return this;
}

Emitter* Emitter::setParticleLocationF(locationF *lf)
{
    if (location_func)
    {
        delete location_func;
    }
    location_func = lf;
    return this;
}

Emitter* Emitter::setParticleColorF(colorF *cf)
{ 
    if (color_func)
    {
        delete color_func;
    }
    color_func = cf;
    return this;
}

Emitter* Emitter::setLocation(const glm::vec3 &l) 
{ 
    loc_ = l; 
    return this;
}

Emitter* Emitter::setOutputRate(float r)
{
    rate_ = r;
    return this;
}


