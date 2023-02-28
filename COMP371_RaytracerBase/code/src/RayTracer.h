#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#define _USE_MATH_DEFINES

#include "../external/json.hpp"
#include <iostream>
#include <string>
#include <vector>
#include "Scene.h"
#include "GeometryVariable.h"
#include "Ray.h"
#include "math.h"
#include <Eigen/Geometry>
#include <array>
#include "HittableList.h"
#include <fstream>
#include <cmath>
#include "PointLight.h"

using color = Eigen::Vector3f;

using namespace std;

class RayTracer
{
private:
    nlohmann::json parsedJson;
    vector<vector<Ray>> initRays(const OutputVariable* output); // Rays for every pixel (Output -> yPosition -> xPosition)
    int getColors(const vector<vector<Ray>>& rays, const OutputVariable* output, const HittableList& hittableList, const vector<LightVariable*> lightVector);
    int save_ppm(std::string file_name, const vector<vector<color>>& colors, int dimx, int dimy);
    void clamp(point3& color);
public:
    RayTracer(nlohmann::json parsedJson);
    ~RayTracer();
    void run();
};

#endif