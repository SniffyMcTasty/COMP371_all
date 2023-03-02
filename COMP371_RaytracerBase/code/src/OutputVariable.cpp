#include "OutputVariable.h"

OutputVariable::OutputVariable(nlohmann::json parsedJson)
{
    this->filename = parsedJson["filename"];
    this->size = parsedJson["size"];
    this->fov = parsedJson["fov"];
    array<float, 3> temp;
    temp = parsedJson["up"];
    this->up << temp.at(0), temp.at(1), temp.at(2);
    this->up.normalize();
    temp = parsedJson["lookat"];
    this->lookat << temp.at(0), temp.at(1), temp.at(2);
    this->lookat.normalize();
    temp = parsedJson["centre"];
    this->centre << temp.at(0), temp.at(1), temp.at(2);
    temp = parsedJson["ai"];
    this->ai << temp.at(0), temp.at(1), temp.at(2);
    temp = parsedJson["bkc"];
    this->bkc << temp.at(0), temp.at(1), temp.at(2);

    /* Optional args */
    try {
        for(unsigned int i : parsedJson["raysperpixel"]) this->raysperpixel.push_back(i);
        if(!this->raysperpixel.empty()) this->raysperpixelInit = true;
    } catch (...) {}
    try {
        this->antialiasing = parsedJson["raysperpixel"];
        this->antialiasingInit = true;
    } catch (...) {}
    try {
        this->twosiderender = parsedJson["twosiderender"];
        this->twosiderenderInit = true;
    } catch (...) {}
    try {
        this->globalillum = parsedJson["globalillum"];
        this->globalillumInit = true;
    } catch (...) {}
}

OutputVariable::~OutputVariable()
{
}

string OutputVariable::toString() const
{
    stringstream ss;
    ss << "\t{";
    ss << "\n\t\tfilename: " << this->filename;
    ss << "\n\t\tsize: ";
    for(float i : this->size) {
        ss << to_string(i) << " ";
    }
    ss << "\n\t\tfov: " << this->fov;
    ss << "\n\t\tup: " << this->up.transpose();
    ss << "\n\t\tlookat: " << this->lookat.transpose();
    ss << "\n\t\tcentre: " << this->centre.transpose();
    ss << "\n\t\tai: " << this->ai.transpose();
    ss << "\n\t\tbkc: " << this->bkc.transpose();
    ss << "\n";

    /* Optional orgs */
    if(raysperpixelInit) {
        ss << "\n\t\traysperpixel: ";
        for(unsigned int i : this->raysperpixel) ss << to_string(i) << " ";
        ss << "\n";
    }
    ss << boolalpha;
    if(antialiasingInit) {
        if(!raysperpixelInit) ss << "\n";
        ss << "\t\tantialiasing: " << this->antialiasing << "\n";
    }
    if(twosiderenderInit) {
        if(!raysperpixelInit && !antialiasingInit) ss << "\n";
        ss << "\t\ttwosiderender: " << this->twosiderender << "\n";
    }
    if(globalillumInit) {
        if(!raysperpixelInit && !antialiasingInit && !twosiderenderInit) ss << "\n";
        ss << "\t\tglobalillum: " << this->globalillum << "\n";
    }

    ss << "\t}";

    return ss.str();
}

array<unsigned int, 2> OutputVariable::getSize() const
{
    return this->size;
}

float OutputVariable::getFov() const
{
    return this->fov;
}

array<vec3, 3> OutputVariable::getCameraVectors() const
{
    return array<vec3, 3>{this->up, this->lookat, this->centre};
}

color OutputVariable::getBkc() const
{
    return this->bkc;
}

string OutputVariable::getFilename() const
{
    return this->filename;
}

color OutputVariable::getAmbientIntensity() const
{
    return this->ai;
}

bool OutputVariable::isGlobalIllum() const
{
    return this->globalillum;
}

bool OutputVariable::isAntialiasing() const
{
    return this->antialiasing;
}

bool OutputVariable::isRaysPerPixelInit() const
{
    return this->raysperpixelInit;
}

vector<unsigned int> OutputVariable::getRaysPerPixel() const
{
    return this->raysperpixel;
}
