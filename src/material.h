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

// Diffuse Lambertian material
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

// Specular metal material
class metal : public Material {
    public:
        metal(const Color& a) : m_albedo(a), m_fuzz(0) {}
        metal(const Color& a, double f) : m_albedo(a), m_fuzz(f < 1 ? f : 1) {}

        virtual bool scatter(
            const Ray& r_in, const hit_record& rec, Color& attenuation, Ray& scattered
        ) const override {
            // r_in: in ray
            // attenuation: factor to decrease radiosity
            // scattered: scattered ray due to reflection (possible fuzzed)
            Vector3f reflected = reflect((r_in.w()).normalized(), rec.n);
            scattered = Ray(rec.p, reflected + m_fuzz * Warp::random_in_unit_sphere());
            attenuation = m_albedo;
            return (scattered.w().dot(rec.n)) > 0;
        }

    public:
        Color m_albedo;
        double m_fuzz;
};

// Dielectric Material
class dielectric : public Material {
    public:
        dielectric(double index_of_refraction) : ir(index_of_refraction) {}

        virtual bool scatter(
            const Ray& r_in, const hit_record& rec, Color& attenuation, Ray& scattered
        ) const override {
            attenuation = Color(1.0, 1.0, 1.0);
            double refraction_ratio = rec.front_face ? (1.0/ir) : ir;

            Vector3f unit_direction = (r_in.w()).normalized();
            Vector3f refracted = refract(unit_direction, rec.n, refraction_ratio);

            double cos_theta = fmin((-unit_direction).dot(rec.n), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

            bool cannot_refract = refraction_ratio * sin_theta > 1.0;
            Vector3f direction;

            if (cannot_refract || reflectance(cos_theta, refraction_ratio) > raytracer::random())
                direction = reflect(unit_direction, rec.n);
            else
                direction = refract(unit_direction, rec.n, refraction_ratio);

            scattered = Ray(rec.p, direction);

            return true;
        }

    public:
        double ir; // Index of Refraction
    
    // Schlick Approximation reflectivity
    private:
        static double reflectance(double cosine, double ref_idx) {
            // Use Schlick's approximation for reflectance.
            auto r0 = (1-ref_idx) / (1+ref_idx);
            r0 = r0*r0;
            return r0 + (1-r0)*pow((1 - cosine),5);
        }

};

RAYTRACER_NAMESPACE_END
#endif

