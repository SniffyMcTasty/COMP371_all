#ifndef AREA_LIGHT_H
#define AREA_LIGHT_H

#include <iostream>
#include <sstream>
#include <string>
#include <array>
#include "../external/json.hpp"
#include "LightVariable.h"

class AreaLight : public LightVariable
{
private:
    array<float, 3> p1, p2, p3, p4;
public:
    AreaLight(nlohmann::json parsedJson);
    ~AreaLight();
    virtual string toString() const override;
};

#endif