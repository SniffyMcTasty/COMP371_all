#ifndef SCENE_H
#define SCENE_H

#include "GeometryVariable.h"
#include <vector>
#include "../external/json.hpp"
#include "SphereGeometry.h"
#include <iostream>
#include "RectangleGeometry.h"
#include <sstream>
#include "LightVariable.h"
#include "PointLight.h"
#include "AreaLight.h"
#include "OutputVariable.h"

using namespace std;

class Scene
{
private:
    vector<GeometryVariable*> geometryVector;
    vector<LightVariable*> lightVector;
    vector<OutputVariable*> outputVector;
public:
    Scene(nlohmann::json parsedJson);
    ~Scene();
    string toString() const;
    vector<OutputVariable*> getOutputVector() const;
    vector<GeometryVariable*> getGeometryVector() const;
    vector<LightVariable*> getLightVector() const;
};

#endif