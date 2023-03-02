#include "AreaLight.h"

AreaLight::AreaLight(nlohmann::json parsedJson) : LightVariable(parsedJson)
{
    this->p1 = parsedJson["p1"];
    this->p2 = parsedJson["p2"];
    this->p3 = parsedJson["p3"];
    this->p4 = parsedJson["p4"];
    try {
        for(unsigned int i : parsedJson["areasampling"]) this->areasampling.push_back(i);
        if(!this->areasampling.empty()) this->areasamplingInit = true;
    } catch (...) {}
}

AreaLight::~AreaLight()
{
}

string AreaLight::toString() const
{
    stringstream ss;
    ss << LightVariable::toString();
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
    ss << "\n";

    if(areasamplingInit) {
        ss << "\n\t\traysperpixel: ";
        for(unsigned int i : this->areasampling) ss << to_string(i) << " ";
        ss << "\n";
    }
    ss << "\t}";

    return ss.str();
}