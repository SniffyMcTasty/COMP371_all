#include "AreaLight.h"

AreaLight::AreaLight(nlohmann::json parsedJson) : LightVariable(parsedJson)
{
    array<float, 3> temp = parsedJson["p1"];
    this->p1 << temp.at(0), temp.at(1), temp.at(2);
    temp = parsedJson["p2"];
    this->p2 << temp.at(0), temp.at(1), temp.at(2);
    temp = parsedJson["p3"];
    this->p3 << temp.at(0), temp.at(1), temp.at(2);
    temp = parsedJson["p4"];
    this->p4 << temp.at(0), temp.at(1), temp.at(2);
    try {
        this->usecenter = parsedJson["usecenter"];
    } catch (...) {}
}

AreaLight::~AreaLight()
{
}

string AreaLight::toString() const
{
    stringstream ss;
    ss << LightVariable::toString();
    ss << "\n\t\tp1: " << this->p1.transpose();
    ss << "\n\t\tp2: " << this->p2.transpose();
    ss << "\n\t\tp3: " << this->p3.transpose();
    ss << "\n\t\tp4: " << this->p1.transpose();
    ss << "\n\t}";

    return ss.str();
}

bool AreaLight::isUseCenter() const
{
    return this->usecenter;
}

point3 AreaLight::getCentre() const
{
    float x = (p1.x() + p2.x() + p3.x() + p4.x()) / 4;
    float y = (p1.y() + p2.y() + p3.y() + p4.y()) / 4;
    float z = (p1.z() + p2.z() + p3.z() + p4.z()) / 4;
    return point3(x, y, z);
}

vector<point3> AreaLight::getPoints()
{
    vector<point3> points;
    points.push_back(this->p1);
    points.push_back(this->p2);
    points.push_back(this->p3);
    points.push_back(this->p4);
    return points;
}
