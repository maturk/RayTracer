#ifndef RAY_H
#define RAY_H

#include "common.h"

// Ray class

class ray {
    public:
    ray(){}
    ray(const Point3f& o, const Vector3f& d): origin(o), direction(d) {}

    // Constant "getter" member functions of ray. Cannot change data member values. 
    Point3f origin() const {return origin;}
    Vector3f w() const {return direction;}

    Point3f at(float t) const { 
        return origin + t * direction;
    }

    public:
    Point3f origin;
    Vector3f direction;
}

#endif