#ifndef SHAPE_H
#define SHAPE_H

#include "common.h"
#include "ray.h"

RAYTRACER_NAMESPACE_BEGIN

class Material;

// Shape hit record
struct hit_record{
    // Surface point
    Point3f p;
    // Surface normal
    Vector3f n;
    // Ray t value
    float t;
    // Pointer to hit material instance
    shared_ptr<Material> mat_ptr;

    bool front_face;

    inline void set_face_normal(const Ray& r, const Vector3f& outward_normal, bool& front_face) {
        front_face = r.w().dot(outward_normal) < 0;
        n = front_face ? outward_normal :-outward_normal;
    }
};

// Abstract base shape class
class Shape {
    public:
        virtual bool hit(const Ray& ray, float t_min, float t_max, hit_record& rec) const = 0;
};

RAYTRACER_NAMESPACE_END
#endif