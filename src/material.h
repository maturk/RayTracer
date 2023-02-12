#ifndef MATERIAL_H
#define MATERIAL_H

#include "common.h"
#include "ray.h"
#include "warp.h"
#include "shape.h"

RAYTRACER_NAMESPACE_BEGIN


// Material abstract class
class Material {
    public:
        virtual bool scatter(
            const Ray& r_in, const hit_record& rec, Color& attenuation, Ray& scattered
            ) const = 0;
};

// Diffuse Lambertian
class lambertian : public Material {
    public:
        lambertian(const Color& a) : m_albedo(a) {}

        bool scatter(
            const Ray& r_in, const hit_record& rec, Color& attenuation, Ray& scattered
        ) const override {
            // Correct Lambertian surface
            Vector3f w = Warp::sampleUniformHemisphere(rec.n);
            scattered = Ray(rec.p, w);
            
            // Incorrect Ch 8.3
            // auto scatter_direction = rec.normal + Warp::random_unit_vector();
            // scattered = Ray(rec.p, scatter_direction);

            attenuation = m_albedo;
            return true;
        }

    public:
        Color m_albedo;
};

class metal : public Material {
    public:
        metal(const Color& a) : albedo(a), fuzz(0) {}
        metal(const Color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

        virtual bool scatter(
            const Ray& r_in, const hit_record& rec, Color& attenuation, Ray& scattered
        ) const override {
            Vector3f reflected = reflect((r_in.w()).normalized(), rec.n);
            scattered = Ray(rec.p, reflected + fuzz * Warp::random_in_unit_sphere());
            attenuation = albedo;
            return (scattered.w().dot(rec.n)) > 0;
        }

    public:
        Color albedo;
        double fuzz;
};

RAYTRACER_NAMESPACE_END
#endif

