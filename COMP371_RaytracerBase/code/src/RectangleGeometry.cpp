#include "RectangleGeometry.h"

RectangleGeometry::RectangleGeometry(nlohmann::json parsedJson) : GeometryVariable(parsedJson)
{
    this->p1 = parsedJson["p1"];
    this->p2 = parsedJson["p2"];
    this->p3 = parsedJson["p3"];
    this->p4 = parsedJson["p4"];
}

RectangleGeometry::~RectangleGeometry()
{
}

string RectangleGeometry::toString() const
{
    stringstream ss;
    ss << GeometryVariable::toString();
    ss << "\n\t\tp1: ";
    for(float i : this->p1) {
        ss << to_string(i) << " ";
    }
    ss << "\n\t\tp2: ";
    for(float i : this->p2) {
        ss << to_string(i) << " ";
    }
    ss << "\n\t\tp3: ";
    for(float i : this->p3) {
        ss << to_string(i) << " ";
    }
    ss << "\n\t\tp4: ";
    for(float i : this->p4) {
        ss << to_string(i) << " ";
    }
    ss << "\n\t}";
    return ss.str();
}
