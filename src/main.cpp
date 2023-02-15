#include <iostream>
#include "imgui.h"
#include "common.h"
#include "color.h"
#include "sphere.h"
#include "hittable_list.h"
#include "camera.h"
#include "warp.h"
#include "material.h"


RAYTRACER_NAMESPACE_BEGIN

Color ray_color(const Ray& r, const Shape& world, int depth) {
    if (depth <= 0) {return Color(0,0,0);}

    hit_record rec;
    if (world.hit(r, M_EPSILON, M_INF, rec)) {
        Ray scattered;
        Color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation.cwiseProduct(ray_color(scattered, world, depth-1));
        return Color(0,0,0);
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
    const int max_depth = 50;
    const bool gamma = true;

    // World
    hittable_list world;
    // Materials
    auto material_ground = make_shared<lambertian>(Color(0.8, 0.8, 0.0));
    auto material_center = make_shared<dielectric>(1.5);
    auto material_left   = make_shared<dielectric>(1.5);
    auto material_right  = make_shared<metal>(Color(0.8, 0.6, 0.2), 1.0);
    // Hittable objects
    world.add(make_shared<Sphere>(Point3f( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<Sphere>(Point3f( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(make_shared<Sphere>(Point3f(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<Sphere>(Point3f( 1.0,    0.0, -1.0),   0.5, material_right));

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
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel, gamma); 
        }
    }
    std::cerr << "\nDone.\n";
    return 0;
}

