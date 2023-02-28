#ifndef SPHERE_GEOMETRY_H
#define SPHERE_GEOMETRY_H

#include "GeometryVariable.h"
#include "../external/json.hpp"
#include <array>
#include <string>
#include <sstream>
#include <iostream>
#include "Ray.h"
#include "Hittable.h"

class SphereGeometry : public GeometryVariable, public Hittable
{
private:
    float radius;
    point3 centre;
public:
    SphereGeometry(nlohmann::json parsedJson);
    ~SphereGeometry();
    virtual string toString() const override;
    point3 getCentre() const;
    float getRadius() const;
    virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;
    virtual vector<point3> getColors() const override;
    virtual vector<float> getColorCoefficients() const override;
    virtual float getPhongCoefficient() const override;
};

#endif