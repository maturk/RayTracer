#include <iostream>
#include "imgui.h"
#include "common.h"
#include "color.h"
#include "sphere.h"
#include "hittable_list.h"
#include "camera.h"

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

Color ray_color(const Ray& r, const Shape& world) {
    hit_record rec;
    if (world.hit(r, 0, M_INF, rec)) {
        return 0.5 * (rec.n + Color(1,1,1));
    }
    Vector3f unit_direction = (r.w()).normalized();
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*Color(1.0, 1.0, 1.0) + t*Color(0.5, 0.7, 1.0);
}
RAYTRACER_NAMESPACE_END

int main(){
    using namespace raytracer;
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;

    // World
    hittable_list world;
    world.add(make_shared<Sphere>(Point3f(0,0,-1), 0.5));
    world.add(make_shared<Sphere>(Point3f(0,-100.5,-1), 100));

    // Camera
    Camera camera;

    // Render
    std::cout << "P3\n" << image_width <<" "<< image_height <<"\n255\n";
    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            Color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + raytracer::random()) / (image_width-1);
                auto v = (j + raytracer::random()) / (image_height-1);
                Ray r = camera.get_ray(u, v);
                pixel_color += ray_color(r, world);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);         
        }
    }
    std::cerr << "\nDone.\n";
    return 0;
}

