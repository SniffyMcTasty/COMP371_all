#include "RayTracer.h"

vector<vector<Ray>> RayTracer::initRays(const OutputVariable* output)
{
    vector<vector<Ray>> rays;
    vector<Ray> tempX;
    unsigned int height = output->getSize().at(1);
    unsigned int width = output->getSize().at(0);
    float aspectRatio = float(width) / float(height);
    array<vec3, 3> cameraVectors = output->getCameraVectors(); // up, lookat, centre
    float fov = output->getFov() * M_PI / 180;
    vec3 w = (cameraVectors.at(2) - cameraVectors.at(1)).normalized();
    vec3 uCamera = cameraVectors.at(0).cross(w).normalized();
    vec3 vCamera = w.cross(uCamera);
    float unitHeight = 2 * tan(fov / 2);
    float unitWidth = aspectRatio * unitHeight;
    point3 lowerLeftCorner = cameraVectors.at(2) - unitWidth*uCamera/2 - unitHeight*vCamera/2 - w;
    for(int i = height-1; i >= 0; i--) {
        for(int j = 0; j < width; j++) {
            float u = float(j) / (width-1);
            float v = float(i) / (height-1);
            tempX.push_back(
                Ray(cameraVectors.at(2),
                lowerLeftCorner + unitWidth*uCamera*u + unitHeight*vCamera*v - cameraVectors.at(2)));
        }
        rays.push_back(tempX);
        tempX.clear();
    }
    return rays;
}

int RayTracer::getColors(const vector<vector<Ray>>& rays, const OutputVariable* output, const HittableList& hittableList, const vector<LightVariable*> lightVector)
{
    vector<vector<color>> colors;
    vector<color> tempX;
    for(vector<Ray> y : rays) {
        for(Ray x : y) {
            color temp = rayColor(x, output, hittableList, lightVector, 0);
            clamp(temp);
            tempX.push_back(temp);
        }
        colors.push_back(tempX);
        tempX.clear();
    }
    return save_ppm(output->getFilename(), colors, output->getSize().at(0), output->getSize().at(1));
}

int RayTracer::save_ppm(std::string file_name, const vector<vector<color>>& colors, int dimx, int dimy) {
   
    ofstream ofs(file_name, ios_base::out | ios_base::binary);
    ofs << "P6" << endl << dimx << ' ' << dimy << endl << "255" << endl;
 
    for (int j = 0; j < dimy; ++j)
        for (int i = 0; i < dimx; ++i)
            ofs << (char) (255.0 * colors.at(j).at(i).x()) <<  (char) (255.0 * colors.at(j).at(i).y()) << (char) (255.0 * colors.at(j).at(i).z());
 
    ofs.close();
 
    return 0;
}

RayTracer::RayTracer(nlohmann::json parsedJson)
{
    this->parsedJson = parsedJson;
}

void RayTracer::clamp(point3& color)
{
    if(color.x() < (float) 0.0) color.x() = 0.0;
    else if(color.x() > (float) 1.0) color.x() = 1.0;
    if(color.y() < (float) 0.0) color.y() = 0.0;
    else if(color.y() > (float) 1.0) color.y() = 1.0;
    if(color.z() < (float) 0.0) color.z() = 0.0;
    else if(color.z() > (float) 1.0) color.z() = 1.0;
}

point3 RayTracer::rayColor(Ray ray, const OutputVariable *output, const HittableList &hittableList, const vector<LightVariable *> lightVector, int bounce)
{
    hit_record rec;
    if(hittableList.hit(ray, 0.01, std::numeric_limits<float>::infinity(), rec)) {
        point3 ambientColor = output->getAmbientIntensity().cwiseProduct(rec.colors.at(0)) * rec.colorCoefficients.at(0);
        point3 diffuseColor;
        diffuseColor << 0, 0, 0;
        point3 specularColor;
        specularColor << 0, 0, 0;
        vec3 V = (-ray.getDirection()).normalized();
        for(const LightVariable* light : lightVector) {
            array<point3, 2> intensities = light->getIntensities(); // id, is
            if(light->getType() == "point") {
                PointLight* pointLight = (PointLight*) light;
                vec3 tempL = pointLight->getCentre() - rec.p;
                if (!hittableList.hitBeforeLight(Ray(rec.p, tempL), 0.01, tempL.norm())) {
                    vec3 L = tempL.normalized();
                    float lambertian = max(rec.normal.dot(L), (float) 0.0);
                    diffuseColor += intensities.at(0).cwiseProduct(rec.colors.at(1)) * rec.colorCoefficients.at(1) * lambertian;
                    float specular = 0.0;
                    if(lambertian > 0.0) {
                        vec3 R = (2 * rec.normal * L.dot(rec.normal) - L).normalized();
                        specular = pow(max(R.dot(V), (float) 0.0), rec.phongCoefficient);
                    }
                    specularColor += intensities.at(1).cwiseProduct(rec.colors.at(2)) * rec.colorCoefficients.at(2) * specular;
                }
            }
        }
        color temp = ambientColor + diffuseColor + specularColor;
        if(bounce < 10 && output->isGlobalIllum()) {
            point3 target = rec.p + rec.normal + randomVectorInHemisphere(rec.normal);
            temp = temp.cwiseProduct(rayColor(Ray(rec.p, target - rec.p), output, hittableList, lightVector, bounce+1));
        }
        return temp;
        
    }
    else return output->getBkc();
}

RayTracer::~RayTracer()
{
}

void RayTracer::run() {
    Scene scene = Scene(this->parsedJson);
    cout << "Scene object created" << endl;
    vector<OutputVariable*> outputs = scene.getOutputVector();
    vector<GeometryVariable*> geometryVector = scene.getGeometryVector();
    for(const OutputVariable* output : outputs) {
        cout << "Rendering output " << output->getFilename() << endl;
        vector<vector<Ray>> rays = initRays(output);
        cout << "\tRays for each pixel have been initialized" << endl;
        HittableList world;
        for(GeometryVariable* geometry : geometryVector) {
            if(geometry->getType() == "sphere") {
                world.add(make_shared<SphereGeometry>(*((SphereGeometry*) geometry)));
            }
            if(geometry->getType() == "rectangle") {
                world.add(make_shared<RectangleGeometry>(*((RectangleGeometry*) geometry)));
            }
        }
        int success = getColors(rays, output, world, scene.getLightVector());
        cout << "\tOutput done" << endl;
        world.clear();
    }
}