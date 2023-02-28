#ifndef GEOMETRY_LIST_H
#define GEOMETRY_LIST_H

#include "Hittable.h"
#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class HittableList {
    public:
        HittableList() {}
        HittableList(shared_ptr<Hittable> object) { add(object); }

        void clear() { objects.clear(); }
        void add(shared_ptr<Hittable> object) { objects.push_back(object); }

        bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const;

    public:
        std::vector<shared_ptr<Hittable>> objects;
};

inline bool HittableList::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
            rec.colors = object->getColors();
            rec.colorCoefficients = object->getColorCoefficients();
            rec.phongCoefficient = object->getPhongCoefficient();
        }
    }

    return hit_anything;
}

#endif