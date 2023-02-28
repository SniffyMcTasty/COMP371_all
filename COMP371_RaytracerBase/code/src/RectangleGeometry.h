#ifndef RECTANGLE_GEOMETRY_H
#define RECTANGLE_GEOMETRY_H

#include "GeometryVariable.h"
#include "../external/json.hpp"
#include <array>
#include <string>
#include <sstream>
#include <iostream>

class RectangleGeometry : public GeometryVariable
{
private:
    array<float, 3> p1, p2, p3, p4;
public:
    RectangleGeometry(nlohmann::json parsedJson);
    ~RectangleGeometry();
    virtual string toString() const override;
};

#endif