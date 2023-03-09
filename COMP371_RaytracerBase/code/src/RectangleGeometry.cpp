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
    vec3 normal = (this->p2 - this->p1).cross(this->p3 - this->p1);
    float d = normal.dot(this->p1);
    float denominator = normal.dot(r.getDirection());
    if(denominator == 0) return false;
    float t = (d - normal.dot(r.getOrigin())) / denominator;
    if(t < t_min || t > t_max) return false;

    point3 P = r.at(t);
    vec3 C;

    vec3 edge1 = this->p2 - this->p1;
    vec3 vp1 = P - this->p1;
    C = edge1.cross(vp1);
    if(normal.dot(C) < 0) return false;

    vec3 edge2 = this->p3 - this->p2;
    vec3 vp2 = P - this->p2;
    C = edge2.cross(vp2);
    if(normal.dot(C) < 0) return false;

    vec3 edgeMid = this->p1 - this->p3;
    vec3 vp3 = P - this->p3;
    C = edgeMid.cross(vp3);
    if(normal.dot(C) < 0) {
        vec3 edge3 = this->p4 - this->p3;
        C = edge3.cross(vp3);
        if(normal.dot(C) < 0) return false;

        vec3 edge4 = this->p1 - this->p4;
        vec3 vp4 = P - this->p4;
        C = edge4.cross(vp4);
        if(normal.dot(C) < 0) return false;
    };

    rec.t = t;
    rec.p = P;
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
