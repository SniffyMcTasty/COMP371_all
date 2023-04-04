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
#include "AreaLight.h"
#ifdef linux
    #include <pthread.h>
#endif
#ifdef _WIN32
    #include <thread>
#endif
#include <future>

using color = Eigen::Vector3f;

using namespace std;

class RayTracer
{
private:
    nlohmann::json parsedJson;
    int initRays(OutputVariable* output, HittableList& hittableList, vector<LightVariable*> lightVector); // Rays for every pixel (Output -> yPosition -> xPosition)
    vector<color> horizontalRays(OutputVariable* output, HittableList& hittableList, vector<LightVariable*> lightVector,
        int i,
        point3 lowerLeftCorner,
        unsigned int width, float unitWidth, vec3 uCamera,
        unsigned int height, float unitHeight, vec3 vCamera,
        point3 origin
    );
    int save_ppm(std::string file_name, const vector<vector<color>>& colors, int dimx, int dimy);
    void multiSample(color& color, unsigned int rpp);
    void clamp(color& color);
    color rayColor(Ray ray, OutputVariable* output, HittableList& hittableList, vector<LightVariable*> lightVector, int maxBounce);
public:
    RayTracer(nlohmann::json parsedJson);
    ~RayTracer();
    void run();
};

#endif