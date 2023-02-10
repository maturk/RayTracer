#ifndef SPHERE_H
#define SPHERE_H

#include "common.h"
#include "shape.h"

RAYTRACER_NAMESPACE_BEGIN

class Sphere : public Shape { // inherit all Shape member methods (public stay public and protected stay protected)
    public:
        Sphere() : m_radius(1), m_center(Point3df(0,0,0) {};
        Sphere(float r, Point3f& center) : m_radius(r), m_center(center) {};

    protected:
        float m_radius;
        Point3f m_center;
};

RAYTRACER_NAMESPACE_END

#endif