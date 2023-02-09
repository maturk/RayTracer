#ifndef RAY_H
#define RAY_H

#include "common.h"

RAYTRACER_NAMESPACE_BEGIN

// Ray class
class Ray {
    public:
    Ray(){}
    Ray(const Point3f& o, const Vector3f& d): origin(o), direction(d.normalized()) {}

    // Constant "getter" member functions of ray. Cannot change data member values. 
    Point3f o() const {return origin;}
    Vector3f w() const {return direction;}
    Point3f at(float t) const { 
        return origin + t * direction;
    }

    public:
    Point3f origin;
    Vector3f direction;
};

RAYTRACER_NAMESPACE_END
#endif