#ifndef WARP_H
#define WARP_H

#include "common.h"

RAYTRACER_NAMESPACE_BEGIN

class Warp {
    public:
    
    inline static Vector3f sampleUniformHemisphere(const Vector3f &pole) {
        // Naive implementation using rejection sampling
        Vector3f v;
        do {
            v.x() = 1.f - 2.f * raytracer::random();
            v.y() = 1.f - 2.f * raytracer::random();
            v.z() = 1.f - 2.f * raytracer::random();
        } while (v.squaredNorm() > 1.f);

        if (v.dot(pole) < 0.f)
            v = -v;
        v /= v.norm();
        return v;
    }
    
    inline static Vector3f randomVector() {
        return Vector3f(raytracer::random(),raytracer::random(),raytracer::random());
    }

    inline static Vector3f randomVector(double min, double max) {
        return Vector3f(raytracer::random(min,max),raytracer::random(min,max),raytracer::random(min,max));
    }

    static Vector3f random_in_unit_sphere() {
        while (true) {
            auto p = randomVector(-1,1);
            if (p.squaredNorm() >= 1) continue;
            return p;
        }
    }
};

RAYTRACER_NAMESPACE_END
#endif