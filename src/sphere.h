#ifndef SPHERE_H
#define SPHERE_H

#include "common.h"
#include "shape.h"

RAYTRACER_NAMESPACE_BEGIN

class Sphere : public Shape { // inherit all Shape member methods (public stay public and protected stay protected)
    public:
        Sphere() {}
        Sphere(Point3f center, float r): m_radius(r), m_center(center) {};
        Sphere(Point3f center, float r, shared_ptr<Material> m) 
                                    : m_radius(r), m_center(center), mat_ptr(m) {};
        

        bool hit(const Ray& ray, float t_min, float t_max, hit_record& rec) const {
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
            rec.mat_ptr = mat_ptr;
            rec.set_face_normal(ray,rec.n, rec.front_face);

            return true;
        }

    protected:
        float m_radius;
        Point3f m_center;
        shared_ptr<Material> mat_ptr; // material pointer (e.g. metal/lambertian)
};

RAYTRACER_NAMESPACE_END

#endif