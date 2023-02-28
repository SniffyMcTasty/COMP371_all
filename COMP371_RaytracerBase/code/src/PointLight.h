#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include <iostream>
#include <sstream>
#include <string>
#include <array>
#include "../external/json.hpp"
#include "LightVariable.h"

using namespace std;

class PointLight : public LightVariable
{
private:
    point3 centre;
public:
    PointLight(nlohmann::json parsedJson);
    ~PointLight();
    virtual string toString() const override;
    point3 getCentre() const;
};

#endif