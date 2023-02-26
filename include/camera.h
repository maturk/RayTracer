#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"
#include "ray.h"
#include "image.h"

RAYTRACER_NAMESPACE_BEGIN

Vector3f random_in_unit_disk() { // generate random vector on surface of lens with unit area
    while (true) {
        auto p = Vector3f(random(-1,1), random(-1,1), 0);
        if (p.norm() >= 1) continue;
        return p;
    }
}

class Camera {
    public:
        Camera( Point3f lookfrom, Point3f lookat, Vector3f vup, double vfov, 
            double aspect_ratio, double aperture, double focus_dist): 
            m_lookfrom(lookfrom), m_lookat(lookat),m_vup(vup), m_vfov(vfov), 
            m_aspect_ratio(aspect_ratio), m_aperture(aperture), m_focus_dist(focus_dist) {

            auto theta = degrees_to_radians(m_vfov);
            auto h = tan(theta/2);
            auto viewport_height = 2.0 * h;
            auto viewport_width = m_aspect_ratio * viewport_height;
            w = (lookfrom - lookat).normalized();
            u = (vup.cross(w)).normalized();
            v = w.cross(u);

            m_origin = lookfrom;
            m_horizontal = focus_dist * viewport_width * u;
            m_vertical = focus_dist * viewport_height * v;
            m_lower_left_corner = m_origin - m_horizontal/2 - m_vertical/2 - focus_dist*w;

            m_lens_radius = aperture / 2;

        }

        Ray get_ray(double s, double t) const {
            Vector3f rd = m_lens_radius * random_in_unit_disk();
            Vector3f offset = u * rd.x() + v * rd.y();
            return Ray(m_origin + offset, m_lower_left_corner + s*m_horizontal + t*m_vertical - m_origin - offset);
        }

        void update(const Settings& settings){
            m_aspect_ratio = settings.aspect_ratio;
            
            auto theta = degrees_to_radians(m_vfov);
            auto h = tan(theta/2);
            auto viewport_height = 2.0 * h;
            auto viewport_width = m_aspect_ratio * viewport_height;

            w = (m_lookfrom - m_lookat).normalized();
            u = (m_vup.cross(w)).normalized();
            v = w.cross(u);

            m_origin = m_lookfrom;
            m_horizontal = m_focus_dist * viewport_width * u;
            m_vertical = m_focus_dist * viewport_height * v;
            m_lower_left_corner = m_origin - m_horizontal/2 - m_vertical/2 - m_focus_dist*w;

            m_lens_radius = m_aperture / 2;
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
        Vector3f u, v, w;
        double m_lens_radius;
        double m_aperture;
        double m_focus_dist;
};

RAYTRACER_NAMESPACE_END
#endif