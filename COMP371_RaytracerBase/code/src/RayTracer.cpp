#include "RayTracer.h"

int RayTracer::initRays(OutputVariable* output, HittableList& hittableList, vector<LightVariable*> lightVector)
{
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
    vector<vector<color>> colors;
    vector<future<vector<color>>> futures;
    for(int i = height-1; i >= 0; i--) {
        futures.push_back(async(launch::async | launch::deferred,
            [output, &hittableList, &lightVector, this]
            (int i, point3 lowerLeftCorner, unsigned int width, float unitWidth, vec3 uCamera, unsigned int height, float unitHeight, vec3 vCamera, point3 origin)
            {
                return horizontalRays(output, hittableList, lightVector, i, lowerLeftCorner, width, unitWidth, uCamera, height, unitHeight, vCamera, origin);
            },
            i,
            lowerLeftCorner,
            width,
            unitWidth,
            uCamera,
            height,
            unitHeight,
            vCamera,
            cameraVectors.at(2)
        ));
    }
    for(int z = 0; z < futures.size(); z++) {
        colors.push_back(futures.at(z).get());
    }
    return save_ppm(output->getFilename(), colors, output->getSize().at(0), output->getSize().at(1));
}

vector<color> RayTracer::horizontalRays(OutputVariable *output, HittableList &hittableList, vector<LightVariable *> lightVector, int i, point3 lowerLeftCorner, unsigned int width, float unitWidth, vec3 uCamera, unsigned int height, float unitHeight, vec3 vCamera, point3 origin)
{
    vector<color> tempX;
    for(int j = 0; j < width; j++) {
        float u, v;
        color pixelColor(0, 0, 0);
        if(output->isGlobalIllum()) {
            u = float(j) / (width-1);
            v = float(i) / (height-1);
            pixelColor += rayColor(
                Ray(origin, lowerLeftCorner + unitWidth*uCamera*u + unitHeight*vCamera*v - origin),
                output,
                hittableList,
                lightVector,
                output->getMaxBounce()
            );
        }
        else if(output->isAntialiasing()) {
            if(output->isRaysPerPixelInit()) {
                vector<unsigned int> rpp = output->getRaysPerPixel();
                if(rpp.size() == 1)
                {
                    for(int k = 0; k < rpp.at(0); k++) {
                        u = (j + randomFloat()) / (width-1);
                        v = (i + randomFloat()) / (height-1);
                        pixelColor += rayColor(
                            Ray(origin, lowerLeftCorner + unitWidth*uCamera*u + unitHeight*vCamera*v - origin),
                            output,
                            hittableList,
                            lightVector,
                            0
                        );
                    }
                    gamma(pixelColor, rpp.at(0));
                }
                else if(rpp.size() == 2) {
                    float strat = 1.0 / rpp.at(0);
                    for(int k = 0; k < rpp.at(0); k++) {
                        for(int l = 0; l < rpp.at(0); l++) {
                            for(int m = 0; m < rpp.at(1); m++) {
                                u = (j + l*strat + randomFloat()) / (width-1);
                                v = (i + k*strat + randomFloat()) / (height-1);
                                pixelColor += rayColor(
                                    Ray(origin, lowerLeftCorner + unitWidth*uCamera*u + unitHeight*vCamera*v - origin),
                                    output,
                                    hittableList,
                                    lightVector,
                                    0
                                );
                            }
                        }
                    }
                    gamma(pixelColor, rpp.at(0) * rpp.at(0) * rpp.at(1));
                }
                else {
                    float strat = 1.0 / rpp.at(0);
                    for(int k = 0; k < rpp.at(0); k++) {
                        for(int l = 0; l < rpp.at(1); l++) {
                            for(int m = 0; m < rpp.at(2); m++) {
                                u = (j + l*strat + randomFloat()) / (width-1);
                                v = (i + k*strat + randomFloat()) / (height-1);
                                pixelColor += rayColor(
                                    Ray(origin, lowerLeftCorner + unitWidth*uCamera*u + unitHeight*vCamera*v - origin),
                                    output,
                                    hittableList,
                                    lightVector,
                                    0
                                );
                            }
                        }
                    }
                    gamma(pixelColor, rpp.at(0) * rpp.at(1) * rpp.at(2));
                }
            }
            // TODO: make default value for rays per pixel if not specified
        } else {
            u = float(j) / (width-1);
            v = float(i) / (height-1);
            pixelColor += rayColor(
                Ray(origin, lowerLeftCorner + unitWidth*uCamera*u + unitHeight*vCamera*v - origin),
                output,
                hittableList,
                lightVector,
                0
            );
        }
        clamp(pixelColor);
        tempX.push_back(pixelColor);
    }
    return tempX;
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

void RayTracer::gamma(color &color, unsigned int rpp)
{
    float scale = 1.0 / rpp;
    color.x() = sqrt(scale * color.x());
    color.y() = sqrt(scale * color.y());
    color.z() = sqrt(scale * color.z());
}

RayTracer::RayTracer(nlohmann::json parsedJson)
{
    this->parsedJson = parsedJson;
}

void RayTracer::clamp(color& color)
{
    if(color.x() < (float) 0.0) color.x() = 0.0;
    else if(color.x() > (float) 1.0) color.x() = 1.0;
    if(color.y() < (float) 0.0) color.y() = 0.0;
    else if(color.y() > (float) 1.0) color.y() = 1.0;
    if(color.z() < (float) 0.0) color.z() = 0.0;
    else if(color.z() > (float) 1.0) color.z() = 1.0;
}

color RayTracer::rayColor(Ray ray, OutputVariable *output, HittableList &hittableList, vector<LightVariable *> lightVector, int maxBounce)
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
            if(light->isUsed()) {
                array<point3, 2> intensities = light->getIntensities(); // id, is
                string lightType = light->getType();
                if(lightType == "point") {
                    vec3 L = (light->getCentre() - rec.p).normalized();
                    if (!hittableList.hitBeforeLight(Ray(rec.p, L), 0.01, std::numeric_limits<float>::infinity())) {
                        float lambertian = max(rec.normal.dot(L), (float) 0.0);
                        diffuseColor += intensities.at(0).cwiseProduct(rec.colors.at(1)) * rec.colorCoefficients.at(1) * lambertian;
                        if(!output->isGlobalIllum()) {
                            float specular = 0.0;
                            if(lambertian > 0.0) {
                                vec3 R = (2 * rec.normal * L.dot(rec.normal) - L).normalized();
                                specular = pow(max(R.dot(V), (float) 0.0), rec.phongCoefficient);
                            }
                            specularColor += intensities.at(1).cwiseProduct(rec.colors.at(2)) * rec.colorCoefficients.at(2) * specular;
                        }
                    }
                } else if (lightType == "area" && output->isGlobalIllum()) {
                    vec3 L = (light->getCentre() - rec.p).normalized();
                    if (!hittableList.hitBeforeLight(Ray(rec.p, L), 0.01, std::numeric_limits<float>::infinity())) {
                        float lambertian = max(rec.normal.dot(L), (float) 0.0);
                        diffuseColor += intensities.at(0).cwiseProduct(rec.colors.at(1)) * rec.colorCoefficients.at(1) * lambertian;
                        if(!output->isGlobalIllum()) {
                            float specular = 0.0;
                            if(lambertian > 0.0) {
                                vec3 R = (2 * rec.normal * L.dot(rec.normal) - L).normalized();
                                specular = pow(max(R.dot(V), (float) 0.0), rec.phongCoefficient);
                            }
                            specularColor += intensities.at(1).cwiseProduct(rec.colors.at(2)) * rec.colorCoefficients.at(2) * specular;
                        }
                    }
                } else {
                    AreaLight* areaLight = (AreaLight*) light;
                }
            }
        }
        color temp = ambientColor + diffuseColor + specularColor;
        if(maxBounce > 0) {
            point3 target = rec.p + rec.normal + randomVectorInHemisphere(rec.normal);
            temp = temp.cwiseProduct(rayColor(Ray(rec.p, target - rec.p), output, hittableList, lightVector, maxBounce-1));
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
    vector<OutputVariable*> outputs = scene.getOutputVector();
    vector<GeometryVariable*> geometryVector = scene.getGeometryVector();
    HittableList world;
    for(GeometryVariable* geometry : geometryVector) {
        if(geometry->getType() == "sphere") {
            world.add(make_shared<SphereGeometry>(*((SphereGeometry*) geometry)));
        }
        if(geometry->getType() == "rectangle") {
            world.add(make_shared<RectangleGeometry>(*((RectangleGeometry*) geometry)));
        }
    }
    time_t tstart, tend;
    for(OutputVariable* output : outputs) {
        cout << "Rendering output " << output->getFilename() << endl;
        tstart = time(0);
        int result = initRays(output, world, scene.getLightVector());
        tend = time(0);
        cout << "Output done in " << difftime(tend, tstart) << " seconds" << endl;
    }
    world.clear();
}