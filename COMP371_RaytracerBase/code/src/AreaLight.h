#ifndef AREA_LIGHT_H
#define AREA_LIGHT_H

#include <iostream>
#include <sstream>
#include <string>
#include <array>
#include "../external/json.hpp"
#include "LightVariable.h"
#include <vector>

class AreaLight : public LightVariable
{
private:
    point3 p1, p2, p3, p4;
    vector<unsigned int> areasampling;
    bool areasamplingInit = false;
    bool usecenter = false;
public:
    AreaLight(nlohmann::json parsedJson);
    ~AreaLight();
    virtual string toString() const override;
    bool isUseCenter() const;
    point3 getCentre() const override;
};

#endif