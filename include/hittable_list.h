#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "common.h"
#include "shape.h"
#include <memory>
#include <vector>

using std::shared_ptr;
using std::vector;

RAYTRACER_NAMESPACE_BEGIN

class hittable_list : public Shape {
    public:
        hittable_list(){};
        hittable_list(shared_ptr<Shape> object) { add(object);}

        void clear() {objects.clear();}
        void add(shared_ptr<Shape> object) {objects.push_back(object);}

        bool hit(const Ray& ray, float t_min, float t_max, hit_record& rec) const {
            hit_record temp_rec;
            bool hit_anything = false;
            auto closest_so_far = t_max;

            for (const auto& object : objects) {
                if (object->hit(ray, t_min, closest_so_far, temp_rec)) {
                    hit_anything = true;
                    closest_so_far = temp_rec.t;
                    rec = temp_rec;
                }
            }
            return hit_anything;
        }

    private:
        vector<shared_ptr<Shape>> objects;
};

RAYTRACER_NAMESPACE_END
#endif