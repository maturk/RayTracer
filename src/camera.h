#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"
#include "ray.h"

RAYTRACER_NAMESPACE_BEGIN

class Camera {
    public:
        Camera() {
            auto aspect_ratio = 16.0 / 9.0;
            auto viewport_height = 2.0;
            auto viewport_width = aspect_ratio * viewport_height;
            auto focal_length = 1.0;

            origin = Point3f(0, 0, 0);
            horizontal = Vector3f(viewport_width, 0.0, 0.0);
            vertical = Vector3f(0.0, viewport_height, 0.0);
            lower_left_corner = origin - horizontal/2 - vertical/2 - Vector3f(0, 0, focal_length);
        }

        Ray get_ray(double u, double v) const {
            return Ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
        }

    private:
        Point3f origin;
        Point3f lower_left_corner;
        Vector3f horizontal;
        Vector3f vertical;
};

RAYTRACER_NAMESPACE_END
#endif