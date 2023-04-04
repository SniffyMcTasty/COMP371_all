#ifndef LIGHT_VARIABLE_H
#define LIGHT_VARIABLE_H

#include <iostream>
#include <sstream>
#include <string>
#include <array>
#include "../external/json.hpp"
#include <Eigen/Core>
#include <Eigen/Dense>

using vec3 = Eigen::Vector3f;
using point3 = Eigen::Vector3f;

using namespace std;

class LightVariable
{
private:
    string type;
    point3 id, is; // Diffuse and specular light intensity [0, 1]

    /* Optional args */
    array<float, 16> transform;
    bool transformInit = false;
    vector<unsigned int> areasampling; // 1, 2 or 3 values
    bool areasamplingInit = false;
    bool used = true;
public:
    LightVariable(nlohmann::json parsedJson);
    ~LightVariable();
    virtual string toString() const;
    string getType() const;
    array<point3, 2> getIntensities() const;
    bool isUsed() const;
    virtual point3 getCentre() const = 0;
    vector<unsigned int> getAreaSampling() const;
    bool isAreaSamplingInit() const;
};

#endif