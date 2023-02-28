#include "PointLight.h"

PointLight::PointLight(nlohmann::json parsedJson) : LightVariable(parsedJson)
{
    array<float, 3> temp = parsedJson["centre"];
    this->centre << temp.at(0), temp.at(1), temp.at(2);
}

PointLight::~PointLight()
{
}

string PointLight::toString() const
{
    stringstream ss;
    ss << LightVariable::toString();
    ss << "\n\t\tcentre: " << this->centre.transpose();
    ss << "\n\t}";
    return ss.str();
}

point3 PointLight::getCentre() const
{
    return this->centre;
}
