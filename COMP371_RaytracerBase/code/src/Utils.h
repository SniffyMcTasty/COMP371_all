#ifndef UTILS_H
#define UTILS_H

#include <random>
#include <Eigen/Core>

using namespace std;
using vec3 = Eigen::Vector3f;

inline float randomFloat()
{
    static uniform_real_distribution<double> distribution(0.0, 1.0);
    static mt19937 generator;
    return distribution(generator);
}

inline float randomFloat(float min, float max) {
    // Returns a random real in [min,max).
    return min + (max-min)*randomFloat();
}

inline vec3 randomVectorInUnitSphere()
{
    while(true) {
        vec3 temp = vec3(randomFloat(-1, 1), randomFloat(-1, 1), randomFloat(-1, 1));
        if(temp.dot(temp) >= 1) continue;
        return temp.normalized();
    }
}

inline vec3 randomVectorInHemisphere(const vec3& normal) {
    vec3 temp = randomVectorInUnitSphere();
    if (temp.dot(normal) > 0.0) // In the same hemisphere as the normal
        return temp;
    else
        return -temp;
}

#endif