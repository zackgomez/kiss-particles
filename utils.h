#pragma once
#include <glm/glm.hpp>

float normalRandom(float mu, float sigma);

void test_random();

glm::vec3 pointOnSphere(float r, const glm::vec3 &pos);
