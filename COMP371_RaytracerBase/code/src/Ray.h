#ifndef RAY_H
#define RAY_H

#include <Eigen/Core>

using vec3 = Eigen::Vector3f;
using point3 = Eigen::Vector3f;
using namespace std;

class Ray
{
private:
    point3 origin;
    vec3 direction;
public:
    Ray() {};
    Ray(const point3& origin, const vec3& direction) : origin(origin), direction(direction) {};
    ~Ray() = default;
    point3 getOrigin() const {return origin;}
    vec3 getDirection() const {return direction;}
    point3 at(float t) const {return origin + t*direction;}
    string toString() const {stringstream ss; ss << fixed << setprecision(3) << "origin: " << origin.transpose() << ", direction: " << direction.transpose(); return ss.str();};
};

#endif