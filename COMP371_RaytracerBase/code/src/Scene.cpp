#include "Scene.h"

Scene::Scene(nlohmann::json parsedJson)
{
    for(nlohmann::json geometryJson : parsedJson["geometry"]) {
        if(geometryJson["type"] == "sphere") this->geometryVector.push_back(new SphereGeometry(geometryJson));
        else if(geometryJson["type"] == "rectangle") this->geometryVector.push_back(new RectangleGeometry(geometryJson));
    }
    for(nlohmann::json lightJson : parsedJson["light"]) {
        if(lightJson["type"] == "point") this->lightVector.push_back(new PointLight(lightJson));
        else if(lightJson["type"] == "area") this->lightVector.push_back(new AreaLight(lightJson));
    }
    for(nlohmann::json outputJson : parsedJson["output"]) {
        this->outputVector.push_back(new OutputVariable(outputJson));
    }
}

Scene::~Scene()
{
    for(GeometryVariable* i : this->geometryVector) delete i;
    for(LightVariable* i : this->lightVector) delete i;
    for(OutputVariable* i : this->outputVector) delete i;
}

string Scene::toString() const
{
    stringstream ss;
    ss << "geometry: [\n";
    for(int i = 0; i < this->geometryVector.size(); i++)
    {
        ss << this->geometryVector.at(i)->toString();
        if(i < this->geometryVector.size() - 1) ss << ",";
        ss << "\n";
    }
    ss << "],\n";
    ss << "light: [\n";
    for (int i = 0; i < this->lightVector.size(); i++)
    {
        ss << this->lightVector.at(i)->toString();
        if(i < this->lightVector.size() - 1) ss << ",";
        ss << "\n";
    }
    ss << "],\n";
    ss << "output:[\n";
    for (int i = 0; i < this->outputVector.size(); i++)
    {
        ss << this->outputVector.at(i)->toString();
        if(i < this->outputVector.size() - 1) ss << ",";
        ss << "\n";
    }
    ss << "]\n";

    return ss.str();
}

vector<OutputVariable *> Scene::getOutputVector() const
{
    return this->outputVector;
}

vector<GeometryVariable *> Scene::getGeometryVector() const
{
    return this->geometryVector;
}

vector<LightVariable *> Scene::getLightVector() const
{
    return this->lightVector;
}
