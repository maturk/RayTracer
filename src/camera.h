#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"
#include "ray.h"
#include "image.h"

RAYTRACER_NAMESPACE_BEGIN

class Camera {
    public:
        Camera( Point3f lookfrom, Point3f lookat, Vector3f vup, double vfov, double aspect_ratio): 
            m_lookfrom(lookfrom), m_lookat(lookat),m_vup(vup), m_vfov(vfov), m_aspect_ratio(aspect_ratio) {

            auto theta = degrees_to_radians(m_vfov);
            auto h = tan(theta/2);
            auto viewport_height = 2.0 * h;
            auto viewport_width = aspect_ratio * viewport_height;

            Vector3f w = (lookfrom - lookat).normalized();
            Vector3f u = (vup.cross(w)).normalized();
            Vector3f v = w.cross(u);

            m_origin = lookfrom;
            m_horizontal = viewport_width * u;
            m_vertical = viewport_height * v;
            m_lower_left_corner = m_origin - m_horizontal/2 - m_vertical/2 - w;

        }

        Ray get_ray(double s, double t) const {
            return Ray(m_origin, m_lower_left_corner + s*m_horizontal + t*m_vertical - m_origin);
        }

        void update(Image::Data& surface){
            m_aspect_ratio = (surface.width/surface.height);

            auto theta = degrees_to_radians(m_vfov);
            auto h = tan(theta/2);
            auto viewport_height = 2.0 * h;
            auto viewport_width = m_aspect_ratio * viewport_height;

            Vector3f w = (m_lookfrom - m_lookat).normalized();
            Vector3f u = (m_vup.cross(w)).normalized();
            Vector3f v = w.cross(u);

            m_origin = m_lookfrom;
            m_horizontal = viewport_width * u;
            m_vertical = viewport_height * v;
            m_lower_left_corner = m_origin - m_horizontal/2 - m_vertical/2 - w;
        }
    private:
        Point3f m_lookfrom;
        Point3f m_lookat;
        Vector3f m_vup;
        double m_aspect_ratio;
        double m_vfov;
        Point3f m_origin;
        Point3f m_lower_left_corner;
        Vector3f m_horizontal;
        Vector3f m_vertical;
};

RAYTRACER_NAMESPACE_END
#endif