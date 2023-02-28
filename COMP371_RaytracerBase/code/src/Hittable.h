#ifndef HITTABLE_H
#define HITTABLE_H

#include "Ray.h"

struct hit_record {
    point3 p;
    vec3 normal;
    double t;
    bool front_face;
    vector<point3> colors;
    vector<float> colorCoefficients;
    float phongCoefficient;

    inline void set_face_normal(const Ray& r, const vec3& outward_normal) {
        front_face = r.getDirection().dot(outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable {
    public:
        virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const = 0;
        virtual vector<point3> getColors() const = 0;
        virtual vector<float> getColorCoefficients() const = 0;
        virtual float getPhongCoefficient() const = 0;
};

#endif