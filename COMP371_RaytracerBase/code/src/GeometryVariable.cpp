#include "GeometryVariable.h"

GeometryVariable::GeometryVariable(nlohmann::json parsedJson)
{
    array<float, 3> temp;
    this->type = parsedJson["type"];
    temp = parsedJson["ac"];
    this->ac << temp.at(0), temp.at(1), temp.at(2);
    temp = parsedJson["dc"];
    this->dc << temp.at(0), temp.at(1), temp.at(2);
    temp = parsedJson["sc"];
    this->sc << temp.at(0), temp.at(1), temp.at(2);
    this->ka = parsedJson["ka"];
    this->kd = parsedJson["kd"];
    this->ks = parsedJson["ks"];
    this->pc = parsedJson["pc"];

    /* Optional args */
    try {
        this->transform = parsedJson["transform"];
        this->transformInit = true;
    } catch (...) {}
}

GeometryVariable::~GeometryVariable()
{

}

string GeometryVariable::toString() const{
    stringstream ss;
    ss << "\t{";
    ss << "\n\t\ttype: " << this->type;
    ss << "\n\t\tac: " << this->ac.transpose();
    ss << "\n\t\tdc: " << this->dc.transpose();
    ss << "\n\t\tsc: " << this->sc.transpose();
    ss << "\n\t\tka: " << this->ka;
    ss << "\n\t\tkd: " << this->kd;
    ss << "\n\t\tks: " << this->ks;
    ss << "\n\t\tpc: " << this->pc;
    ss << "\n";

    /* Optional args */
    if(this->transformInit) {
        ss << "\n\t\ttransform: \n\t\t\t";
        for(int i = 0; i < this->transform.size(); i++) {
            if(i % 4 == 0) ss << "\n\t\t\t";
            ss << to_string(this->transform.at(i));
        }
        ss << "\n";
    }

    return ss.str();
}

string GeometryVariable::getType() const
{
    return this->type;
}

vector<point3> GeometryVariable::getColors() const
{
    vector<point3> temp;
    temp.push_back(this->ac);
    temp.push_back(this->dc);
    temp.push_back(this->sc);
    return temp;
}

vector<float> GeometryVariable::getColorCoefficients() const
{
    vector<float> temp;
    temp.push_back(this->ka);
    temp.push_back(this->kd);
    temp.push_back(this->ks);
    return temp;
}

float GeometryVariable::getPhongCoefficient() const
{
    return this->pc;
}
