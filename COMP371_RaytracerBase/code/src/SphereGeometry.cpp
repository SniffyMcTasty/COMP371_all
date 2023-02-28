#include "SphereGeometry.h"

SphereGeometry::SphereGeometry(nlohmann::json parsedJson) : GeometryVariable(parsedJson)
{
    this->radius = parsedJson["radius"];
    array<float, 3> temp = parsedJson["centre"];
    this->centre << temp.at(0), temp.at(1), temp.at(2);
}

SphereGeometry::~SphereGeometry()
{
}

string SphereGeometry::toString() const
{
    stringstream ss;
    ss << GeometryVariable::toString();
    ss << "\n\t\tradius: " << this->radius;
    ss << "\n\t\tcentre: " << this->centre.transpose();
    ss << "\n\t}";
    return ss.str();
}

point3 SphereGeometry::getCentre() const
{
    return this->centre;
}

float SphereGeometry::getRadius() const
{
    return this->radius;
}

bool SphereGeometry::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const
{
    vec3 oc = r.getOrigin() - this->centre;
    float a = r.getDirection().dot(r.getDirection());
    float half_b = oc.dot(r.getDirection());
    float c = oc.dot(oc) - radius*radius;

    float discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false;
    float sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    float root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - this->centre) / this->radius;
    rec.set_face_normal(r, outward_normal);

    return true;
}

vector<point3> SphereGeometry::getColors() const
{
    return GeometryVariable::getColors();
}

vector<float> SphereGeometry::getColorCoefficients() const
{
    return GeometryVariable::getColorCoefficients();
}

float SphereGeometry::getPhongCoefficient() const
{
    return GeometryVariable::getPhongCoefficient();
}
