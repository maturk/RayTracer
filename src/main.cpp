#include <iostream>
#include "imgui.h"
#include "common.h"
#include "color.h"
#include "sphere.h"
#include "hittable_list.h"
#include "camera.h"
#include "warp.h"
#include "material.h"
#include "gui.cpp"
#define STB_IMAGE_IMPLEMENTATION
#include "image.cpp"


RAYTRACER_NAMESPACE_BEGIN

Color ray_Color(const Ray& r, const Shape& world, int depth) {
    if (depth <= 0) {return Color(0,0,0);}

    hit_record rec;
    if (world.hit(r, M_EPSILON, M_INF, rec)) {
        Ray scattered;
        Color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation.cwiseProduct(ray_Color(scattered, world, depth-1));
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
    const int image_width = 800;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 3;
    const int max_depth = 10;
    const bool gamma = true;

    // World
    hittable_list world;

    // Materials
   // auto material_ground = make_shared<lambertian>(Color(0.8, 0.8, 0.0));
   // auto material_center = make_shared<lambertian>(Color(0.1, 0.2, 0.5));
   // auto material_left   = make_shared<dielectric>(1.5);
   // auto material_right  = make_shared<metal>(Color(0.8, 0.6, 0.2), 0.0);

   // world.add(make_shared<Sphere>(Point3f( 0.0, -100.5, -1.0), 100.0, material_ground));
   // world.add(make_shared<Sphere>(Point3f( 0.0,    0.0, -1.0),   0.5, material_center));
   // world.add(make_shared<Sphere>(Point3f(-1.0,    0.0, -1.0),   0.5, material_left));
   // world.add(make_shared<Sphere>(Point3f(-1.0,    0.0, -1.0), -0.45, material_left));
   // world.add(make_shared<Sphere>(Point3f( 1.0,    0.0, -1.0),   0.5, material_right));

    auto material_ground = make_shared<lambertian>(Color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(Color(0.1, 0.2, 0.5));
    auto material_left   = make_shared<dielectric>(1.5);
    auto material_right  = make_shared<metal>(Color(0.8, 0.6, 0.2), 0.0);

    world.add(make_shared<Sphere>(Point3f( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<Sphere>(Point3f( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(make_shared<Sphere>(Point3f(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<Sphere>(Point3f(-1.0,    0.0, -1.0),  -0.4, material_left));
    world.add(make_shared<Sphere>(Point3f( 1.0,    0.0, -1.0),   0.5, material_right));

    // Camera
    Camera camera(Point3f(-2,2,1), Point3f(0,0,-1), Vector3f(0,1,0), 20, aspect_ratio);
    //GUI and image
    GUI gui(image_width,image_height);
    Image image(image_width,image_height);

    while (gui.open()){
        gui.start(image);
        image.update();
        image.display();
        gui.end();
        if (image.m_surface.render == true){ 
            gui.start(image);
            for (int s = 1; s <= samples_per_pixel; s++) {
                std::cerr << "\rSamples per pixel remaining: " << samples_per_pixel-(s-1) << ' ' << std::flush;
                camera.update(image.m_surface); // update camera viewport
                for (int y = 0; y < image.m_surface.height; y++){
                    for (int x = 0; x < image.m_surface.width; x++){
                        auto u = (x + raytracer::random()) / (image.m_surface.width-1);
                        auto v = (y + raytracer::random()) / (image.m_surface.height-1);
                        Ray r = camera.get_ray(u, v);
                        Color pixel_Color = ray_Color(r, world, max_depth);
                        pixel_Color = pixel_Color.cwiseSqrt(); // Gamma correction
                        image.m_surface.pixels[(y*image.m_surface.width +x) * 4 +0] = 
                            ((image.m_surface.pixels[(y*image.m_surface.width +x) * 4 +0])*((s-1)) + 255*pixel_Color.x())/s;
                        image.m_surface.pixels[(y*image.m_surface.width +x) * 4 +1] =
                            ((image.m_surface.pixels[(y*image.m_surface.width +x) * 4 +1])*((s-1)) + 255*pixel_Color.y())/s;
                        image.m_surface.pixels[(y*image.m_surface.width +x) * 4 +2] =
                            ((image.m_surface.pixels[(y*image.m_surface.width +x) * 4 +2])*((s-1)) + 255*pixel_Color.z())/s;
                        image.m_surface.pixels[(y*image.m_surface.width +x) * 4 +3] = (float)0;
                    }
                }
                image.update();
                image.display();
                gui.end();
            }
        }
        std::cerr<<std::flush;
        std::cerr << "\rFinished rendering: "<< std::flush;
        image.m_surface.render = false;
        image.update();
        image.display();
        gui.end();
    }
    image.destroy();
    gui.destroy();

    std::cerr << "\nDone.\n";
    return 0;
}

