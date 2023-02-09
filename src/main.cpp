#include <iostream>
#include "imgui.h"
#include "common.h"
#include "ray.h"
#include "color.h"

RAYTRACER_NAMESPACE_BEGIN

float hit_sphere(const Point3f& center, double radius, const Ray& r) {
    Vector3f oc = r.o() - center;
    auto a = r.w().dot(r.w());
    auto b = 2.0 * oc.dot(r.w());
    auto c = oc.dot(oc) - radius*radius;
    auto discriminant = b*b - 4*a*c;
    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-b - sqrt(discriminant) ) / (2.0*a); // smallest t
    }
}


Color ray_color(const Ray& r) {
    auto t = hit_sphere(Point3f(0,0,-1), 0.5, r);
    if (t > 0.0) {
        Vector3f n = (r.at(t) - Vector3f(0,0,-1)).normalized(); // (P - C).norm() = normal at surface
        return 0.5*Color(n.x()+1, n.y()+1, n.z()+1); // mapping [-1,1] coordinates to [0,1] interval
    }
    Vector3f unit_direction = (r.w());
    t = 0.5*(unit_direction.y() + 1.0); // y coordinate mapped from [-1,1] to [0,1]
    return (1.0-t)*Color(1.0, 1.0, 1.0) + t*Color(0.2, 0.2, 1.0);
}

RAYTRACER_NAMESPACE_END

int main(){
    using namespace raytracer;
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    Vector3f origin = Point3f(0, 0, 0);
    Vector3f horizontal = Vector3f(viewport_width, 0, 0);
    Vector3f vertical = Vector3f(0, viewport_height, 0);
    Vector3f lower_left_corner = origin - horizontal/2 - vertical/2 - Vector3f(0, 0, focal_length);

    // Render
    std::cout << "P3\n" << image_width <<" "<< image_height <<"\n255\n";
    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto u = float(i) / (image_width-1);
            auto v = float(j) / (image_height-1);
            
            Ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
            Color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);         
        }
    }
    std::cerr << "\nDone.\n";
    return 0;
}
