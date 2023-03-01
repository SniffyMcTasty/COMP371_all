#ifndef RECTANGLE_GEOMETRY_H
#define RECTANGLE_GEOMETRY_H

#include "GeometryVariable.h"
#include "../external/json.hpp"
#include <array>
#include <string>
#include <sstream>
#include <iostream>
#include "Hittable.h"

class RectangleGeometry : public GeometryVariable, public Hittable
{
private:
    point3 p1, p2, p3, p4;
public:
    RectangleGeometry(nlohmann::json parsedJson);
    ~RectangleGeometry();
    virtual string toString() const override;
    virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;
    vector<point3> getColors() const override;
    vector<float> getColorCoefficients() const override;
    float getPhongCoefficient() const override;
};

#endif