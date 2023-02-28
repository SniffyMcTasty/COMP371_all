#ifndef GEOMETRY_VARIABLE_H
#define GEOMETRY_VARIABLE_H

#include "../external/json.hpp"
#include <string>
#include <array>
#include <sstream>
#include <iostream>
#include <Eigen/Core>
#include <Eigen/Dense>
#include "Ray.h"

using vec3 = Eigen::Vector3f;
using point3 = Eigen::Vector3f;

using namespace std;

class GeometryVariable
{
private:
    string type;
    point3 ac, dc, sc; // Ambient, diffuse and specular reflection color of a light source [0, 1]
    float ka, kd, ks; // Ambient, diffuse and specular reflection coefficent [0, 1]
    float pc; // Phong coefficient (> 1)

    /* Optional args */
    array<float, 16> transform;
    bool transformInit = false;
public:
    GeometryVariable(nlohmann::json parsedJson);
    ~GeometryVariable();
    virtual string toString() const;
    string getType() const;
    vector<point3> getColors() const;
    vector<float> getColorCoefficients() const;
    float getPhongCoefficient() const;
};

#endif