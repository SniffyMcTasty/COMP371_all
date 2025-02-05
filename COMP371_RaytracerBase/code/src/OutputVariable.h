#ifndef OUTPUT_VARIABLE_H
#define OUTPUT_VARIABLE_H

#include <iostream>
#include <sstream>
#include <string>
#include <array>
#include "../external/json.hpp"
#include <Eigen/Core>
#include <Eigen/Dense>
#include "Utils.h"

using vec3 = Eigen::Vector3f;
using color = Eigen::Vector3f;

using namespace std;

class OutputVariable
{
private:
    string filename; // ppm file for image output
    array<unsigned int, 2> size; // Resolution of image
    float fov; // Angle of FoV
    vec3 up, lookat, centre; // Camera vectors
    color ai, bkc; // Ambient intensity, background color [0, 1]


    /* Optional args */
    vector<unsigned int> raysperpixel; // 1, 2 or 3 values
    bool raysperpixelInit = false;
    bool antialiasing = false;
    bool antialiasingInit = false;
    bool twosiderender;
    bool twosiderenderInit = false;
    bool globalillum;
    bool globalillumInit = false;
    unsigned int maxbounce = MAX_BOUNCE;
    bool maxbounceInit = false;
    float probterminate = 1.0;
    bool probterminateInit = false;
public:
    OutputVariable(nlohmann::json parsedJson);
    ~OutputVariable();
    string toString() const;
    array<unsigned int, 2> getSize() const;
    float getFov() const;
    array<vec3, 3> getCameraVectors() const;
    color getBkc() const;
    string getFilename() const;
    color getAmbientIntensity() const;
    bool isGlobalIllum() const;
    bool isAntialiasing() const;
    bool isRaysPerPixelInit() const;
    vector<unsigned int> getRaysPerPixel() const;
    unsigned int getMaxBounce() const;
    float getProbTerminate() const;
};

#endif