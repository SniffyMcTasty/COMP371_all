#include "RectangleGeometry.h"

RectangleGeometry::RectangleGeometry(nlohmann::json parsedJson) : GeometryVariable(parsedJson)
{
    array<float, 3> temp = parsedJson["p1"];
    this->p1 << temp.at(0), temp.at(1), temp.at(2);
    temp = parsedJson["p2"];
    this->p2 << temp.at(0), temp.at(1), temp.at(2);
    temp = parsedJson["p3"];
    this->p3 << temp.at(0), temp.at(1), temp.at(2);
    temp = parsedJson["p4"];
    this->p4 << temp.at(0), temp.at(1), temp.at(2);
}

RectangleGeometry::~RectangleGeometry()
{
}

string RectangleGeometry::toString() const
{
    stringstream ss;
    ss << GeometryVariable::toString();
    ss << "\n\t\tp1: " << this->p1.transpose();
    ss << "\n\t\tp2: " << this->p2.transpose();
    ss << "\n\t\tp3: " << this->p3.transpose();
    ss << "\n\t\tp4: " << this->p4.transpose();
    ss << "\n\t}";
    return ss.str();
}

bool RectangleGeometry::hit(const Ray &r, double t_min, double t_max, hit_record &rec) const
{
    vec3 normal = (this->p2 - this->p1).cross(this->p3 - this->p1).normalized();
    float d = normal.dot(this->p1);
    float denominator = normal.dot(r.getDirection());
    if(denominator == 0) return false;
    float t = (d - normal.dot(r.getOrigin())) / denominator;
    if(t < t_min || t > t_max) return false;

    point3 Q = r.at(t);
    float outsideP1P2 = (this->p2 - this->p1).cross(Q - this->p1).dot(normal);
    float outsideP2P3 = (this->p3 - this->p2).cross(Q - this->p2).dot(normal);
    float outsideP3P1 = (this->p1 - this->p3).cross(Q - this->p3).dot(normal);
    if(outsideP1P2 < 0 || outsideP2P3 < 0) return false;
    if(outsideP3P1 < 0) {
        float outsideP3P4 = (this->p4 - this->p3).cross(Q - this->p3).dot(normal);
        float outsideP4P1 = (this->p1 - this->p4).cross(Q - this->p4).dot(normal);
        if(outsideP3P4 < 0 || outsideP4P1 < 0) return false;
    }

    rec.t = t;
    rec.p = Q;
    rec.set_face_normal(r, normal);

    return true;
}

vector<point3> RectangleGeometry::getColors() const
{
    return GeometryVariable::getColors();
}

vector<float> RectangleGeometry::getColorCoefficients() const
{
    return GeometryVariable::getColorCoefficients();
}

float RectangleGeometry::getPhongCoefficient() const
{
    return GeometryVariable::getPhongCoefficient();
}
