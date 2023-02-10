#ifndef SPHERE_H
#define SPHERE_H

#include "common.h"
#include "shape.h"

RAYTRACER_NAMESPACE_BEGIN

class Sphere : public Shape { // inherit all Shape member methods (public stay public and protected stay protected)
    public:
        Sphere() : m_radius(1), m_center(Point3f(0,0,0)) {};
        Sphere(float r, Point3f& center) : m_radius(r), m_center(center) {};

        bool hit(const Ray& ray, float t_min, float t_max, hit_record& rec) {
            Vector3f oc = ray.o() - m_center;
            auto a = ray.w().squaredNorm();
            auto half_b = oc.dot(ray.w());
            auto c = oc.squaredNorm() - m_radius*m_radius;

            auto discriminant = half_b*half_b - a*c;
            if (discriminant < 0) return false;
            auto sqrtd = sqrt(discriminant);

            // Find the nearest root that lies in the acceptable range.
            auto root = (-half_b - sqrtd) / a;
            if (root < t_min || t_max < root) {
                root = (-half_b + sqrtd) / a;
                if (root < t_min || t_max < root)
                    return false;
            }

            rec.t = root;
            rec.p = ray.at(rec.t);
            rec.n = (rec.p - m_center) / m_radius; // why not normalized

            return true;
        }
    protected:
        float m_radius;
        Point3f m_center;
};

RAYTRACER_NAMESPACE_END

#endif