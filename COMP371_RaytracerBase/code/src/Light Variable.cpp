#include "LightVariable.h"

LightVariable::LightVariable(nlohmann::json parsedJson)
{
    this->type = parsedJson["type"];
    array<float, 3> temp = parsedJson["id"];
    this->id << temp.at(0), temp.at(1), temp.at(2);
    temp = parsedJson["is"];
    this->is << temp.at(0), temp.at(1), temp.at(2);

    /* Optional args */
    try {
        this->transform = parsedJson["transform"];
        this->transformInit = true;
    } catch (...) {}
    try {
        for(unsigned int i : parsedJson["areasampling"]) this->areasampling.push_back(i);
        if(!this->areasampling.empty()) this->areasamplingInit = true;
    } catch (...) {}
}

LightVariable::~LightVariable()
{
}

string LightVariable::toString() const
{
    stringstream ss;
    ss << "\t{";
    ss << "\n\t\ttype: " << this->type;
    ss << "\n\t\tid: " << this->id.transpose();
    ss << "\n\t\tis: " << this->is.transpose();
    ss << "\n";

    /* Optional args */
    if(this->transformInit) {
        ss << "\n\t\ttransform: \n\t\t\t";
        for(int i = 0; i < this->transform.size(); i++) {
            if(i % 4 == 0) ss << "\n\t\t\t";
            ss << to_string(this->transform.at(i)) << " ";
        }
        if(!this->areasamplingInit) ss << "\n";
    }
    if(this->areasamplingInit) {
        ss << "\n\t\tareasampling: ";
        for(unsigned int i : this->areasampling) ss << to_string(i) << " ";
        ss << "\n";
    }

    return ss.str();
}

string LightVariable::getType() const
{
    return this->type;
}

array<point3, 2> LightVariable::getIntensities() const
{
    return array<point3, 2>{this->id, this->is};
}
