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
    vec3 w = cameraVectors.at(2) - cameraVectors.at(1);
    vec3 uCamera = cameraVectors.at(0).cross(w);
    vec3 vCamera = w.cross(uCamera);
    float unitHeight = 2 * tan(fov / 2);
    float unitWidth = aspectRatio * unitHeight;
    point3 lowerLeftCorner = cameraVectors.at(2) - unitWidth/2*uCamera - unitHeight/2*vCamera - w;
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
            hit_record rec;
            if(hittableList.hit(x, 0.001, std::numeric_limits<double>::infinity(), rec)) {
                point3 ambientColor = output->getAmbientIntensity().cwiseProduct(rec.colors.at(0)) * rec.colorCoefficients.at(0);
                point3 diffuseColor;
                diffuseColor << 0, 0, 0;
                point3 specularColor;
                specularColor << 0, 0, 0;
                vec3 V = (-x.getDirection()).normalized();
                for(const LightVariable* light : lightVector) {
                    array<point3, 2> intensities = light->getIntensities(); // id, is
                    if(light->getType() == "point") {
                        PointLight* pointLight = (PointLight*) light;
                        vec3 L = (pointLight->getCentre() - rec.p).normalized();
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
                point3 temp = ambientColor + diffuseColor + specularColor;
                clamp(temp);
                tempX.push_back(temp);
            }
            else tempX.push_back(output->getBkc());
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