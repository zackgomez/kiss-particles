#pragma once
#include <glm/glm.hpp>

float normalRandom(float mu, float sigma);
float randomFloat(float min = 0.f, float max = 1.f);

void test_random();

glm::vec3 pointOnSphere(float r, const glm::vec3 &pos);
