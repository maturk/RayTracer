#include <iostream>
#include "imgui.h"
#include "common.h"
#include "color.h"
#include "sphere.h"
#include "hittable_list.h"
#include "camera.h"
#include "warp.h"

RAYTRACER_NAMESPACE_BEGIN


Color ray_color(const Ray& r, const Shape& world, int depth) {
    if (depth <= 0) {return Color(0,0,0);}

    hit_record rec;
    float t_max = 10;
    if (world.hit(r, M_EPSILON, M_INF, rec)) {
        //Point3f sample = Point3f(raytracer::random(), raytracer::random(), raytracer::random());
        //Vector3f w = Warp::sampleUniformHemisphere(sample, rec.n);
        Point3f target = rec.p + rec.n + Warp::random_in_unit_sphere();
        return 0.5 * ray_color(Ray(rec.p, target - rec.p), world, depth-1);
    }
    //std::cout<<"here before unit"<<std::endl;
    Vector3f unit_direction = (r.w()).normalized();
    //std::cout<<"unit direction"<<"\n"<<unit_direction<<std::endl;
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
            //std::cout<<"here"<<std::endl;        
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + raytracer::random()) / (image_width-1);
                auto v = (j + raytracer::random()) / (image_height-1);
                Ray r = camera.get_ray(u, v);
                //std::cout<< r.o() <<r.w() <<std::endl; 
                pixel_color += ray_color(r, world, max_depth);
                //std::cout<<"here5"<<std::endl; 
            }
            write_color(std::cout, pixel_color, samples_per_pixel); 
        }
    }
    std::cerr << "\nDone.\n";
    return 0;
}

