#include "utils.h"
#define _USE_MATH_DEFINES
#include <cstdio>
#include <cmath>
#include <cstdlib>

// Approximate a normal variable.
// http://en.wikipedia.org/wiki/Normal_distribution#Generating_values_from_normal_distribution
// A sum of several uniform variables is like a normal distr.
// (central limit theorom)
float normalRandom(float mu, float sigma)
{
    float ans = 0.f;
    for (int i = 0; i < 6; i++)
    {
        ans += static_cast<float>(rand()) / RAND_MAX;
    }
    return mu + (ans - 3) * sigma;
}

float randomFloat(float min, float max)
{
    return static_cast<float>(rand()) / RAND_MAX * (max - min) + min;
}


float frand() 
{
    return static_cast <float>(rand()) / RAND_MAX;
}

void test_random() {
    float s1 = 1;
    float mu = 7;
    float sum = 0.f;
    for (int i = 0; i < 100000; i++)
    {
        sum += normalRandom(s1, mu);
    }
    sum /= 100000;
    printf("Average is %f\n", sum);

    s1 = 10;
    mu = -100;
    for (int i = 0; i < 10; i++)
    {
        printf("normalRandom(mu = %f, sigma = %f) = %f\n", mu, s1, normalRandom(mu, s1));
    }
}

glm::vec3 pointOnSphere(float r, const glm::vec3 &pos)
{
    float theta = M_PI * float(rand()) / RAND_MAX;
    float phi = 2 * M_PI * float(rand()) / RAND_MAX;
    glm::vec3 ans;
    ans.x = r * sin(theta) * cos(phi);
    ans.y = r * sin(theta) * sin(phi);
    ans.z = r * cos(theta);
    return pos + ans;
}
