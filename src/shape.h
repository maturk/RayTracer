#ifndef SHAPE_H
#define SHAPE_H

#include "common.h"
#include "ray.h"

RAYTRACER_NAMESPACE_BEGIN

// Shape hit record
struct hit_record{
    // Surface point
    Point3f p;
    // Surface normal
    Vector3f n;
    // Ray t value
    float t;
};

// Abstract shape class
class Shape {
    public:
        virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};

RAYTRACER_NAMESPACE_END
#endif