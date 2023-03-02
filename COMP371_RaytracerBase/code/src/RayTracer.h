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
#include "Utils.h"
#include <ctime>
#include <thread>
#include <future>

using color = Eigen::Vector3f;

using namespace std;

class RayTracer
{
private:
    nlohmann::json parsedJson;
    int initRays(OutputVariable* output, HittableList& hittableList, vector<LightVariable*> lightVector); // Rays for every pixel (Output -> yPosition -> xPosition)
    int save_ppm(std::string file_name, const vector<vector<color>>& colors, int dimx, int dimy);
    void gamma(color& color, unsigned int rpp);
    void clamp(color& color);
    void rayColor(Ray ray, OutputVariable* output, HittableList& hittableList, vector<LightVariable*> lightVector, int maxBounce, promise<color> &&p);
public:
    RayTracer(nlohmann::json parsedJson);
    ~RayTracer();
    void run();
};

#endif