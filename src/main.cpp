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
#include "image.h"
#include "timer.h"


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

hittable_list random_scene() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(Color(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(Point3f(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random();
            Point3f center(a + 0.9*random(), 0.2, b + 0.9*random());

            if ((center - Point3f(4, 0.2, 0)).norm() > 0.9) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Color(random() * random(),random() * random(),random() * random());
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Color(random(0.5, 1),random(0.5, 1),random(0.5, 1));
                    auto fuzz = random(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<Sphere>(Point3f(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(Color(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(Point3f(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(Point3f(4, 1, 0), 1.0, material3));

    return world;
}

RAYTRACER_NAMESPACE_END

int main(){
    using namespace raytracer;

    // Image
    Settings settings;

    // World
    //hittable_list world;
    auto world = random_scene();

    // Materials
    //auto material_ground = make_shared<lambertian>(Color(0.8, 0.8, 0.0));
    //auto material_center = make_shared<lambertian>(Color(0.1, 0.2, 0.5));
    //auto material_left   = make_shared<dielectric>(1.5);
    //auto material_right  = make_shared<metal>(Color(0.8, 0.6, 0.2), 0.0);
    //
    //world.add(make_shared<Sphere>(Point3f( 0.0, -100.5, -1.0), 100.0, material_ground));
    //world.add(make_shared<Sphere>(Point3f( 0.0,    0.0, -1.0),   0.5, material_center));
    //world.add(make_shared<Sphere>(Point3f(-1.0,    0.0, -1.0),   0.5, material_left));
    //world.add(make_shared<Sphere>(Point3f(-1.0,    0.0, -1.0), -0.45, material_left));
    //world.add(make_shared<Sphere>(Point3f( 1.0,    0.0, -1.0),   0.5, material_right));
    
    //auto material_ground = make_shared<lambertian>(Color(0.8, 0.8, 0.0));
    //auto material_center = make_shared<lambertian>(Color(0.1, 0.2, 0.5));
    //auto material_left   = make_shared<dielectric>(1.5);
    //auto material_right  = make_shared<metal>(Color(0.8, 0.6, 0.2), 0.0);
    //world.add(make_shared<Sphere>(Point3f( 0.0, -100.5, -1.0), 100.0, material_ground));
    //world.add(make_shared<Sphere>(Point3f( 0.0,    0.0, -1.0),   0.5, material_center));
    //world.add(make_shared<Sphere>(Point3f(-1.0,    0.0, -1.0),   0.5, material_left));
    //world.add(make_shared<Sphere>(Point3f(-1.0,    0.0, -1.0),  -0.4, material_left));
    //world.add(make_shared<Sphere>(Point3f( 1.0,    0.0, -1.0),   0.5, material_right));

    // Camera
    Point3f lookfrom(13,2,3);
    Point3f lookat(0,0,0);
    Vector3f vup(0,1,0);
    double dist_to_focus = 10;
    double aperture = 0.1;

    Camera camera(lookfrom, lookat, vup, 20, settings.aspect_ratio, aperture, dist_to_focus);

    // GUI and Image
    GUI gui(settings.image_width,settings.image_height);
    Image image(settings.image_width,settings.image_height);

    // Timer
    Timer timer;

    while (gui.open()){
        gui.start(image, settings);
        if (image.m_surface.save){save_image(image); image.m_surface.save = false;}
        //image.update();
        //image.display(settings);
        //gui.end();
        while (image.m_surface.render == true){ 
            for (int s = 1; s <= settings.samples_per_pixel; s++) {
                std::cerr << "\rSamples per pixel remaining: " << settings.samples_per_pixel-(s-1) << "\n " << std::flush;
                camera.update(settings); // update camera viewport
                timer.reset();
                gui.start(image, settings);
                #pragma omp parallel for ordered schedule(dynamic)
                for (int y = 0; y < image.m_surface.height; y++){
                    for (int x = 0; x < image.m_surface.width; x++){
                        auto u = (x + raytracer::random()) / (image.m_surface.width-1);
                        auto v = (y + raytracer::random()) / (image.m_surface.height-1);
                        Ray r = camera.get_ray(u, v);
                        
                        Color pixel_Color = ray_Color(r, world, settings.max_depth);
                        if (settings.gamma){pixel_Color = pixel_Color.cwiseSqrt(); } // Gamma correction
                        image.m_surface.pixels[(y*image.m_surface.width +x) * 4 +0] = 
                            ((image.m_surface.pixels[(y*image.m_surface.width +x) * 4 +0])*((s-1)) + 255*pixel_Color.x())/s;
                        image.m_surface.pixels[(y*image.m_surface.width +x) * 4 +1] =
                            ((image.m_surface.pixels[(y*image.m_surface.width +x) * 4 +1])*((s-1)) + 255*pixel_Color.y())/s;
                        image.m_surface.pixels[(y*image.m_surface.width +x) * 4 +2] =
                            ((image.m_surface.pixels[(y*image.m_surface.width +x) * 4 +2])*((s-1)) + 255*pixel_Color.z())/s;
                        image.m_surface.pixels[(y*image.m_surface.width +x) * 4 +3] = (float)0;
                        
                        if (image.m_surface.save){save_image(image); image.m_surface.save = false;}
                    }
                }
                float time = timer.end();
                std::cerr << "\r \t Time taken to render this sample: " << time << "\n " << std::flush;
                image.update();
                image.display(settings);
                gui.end();
            }
           image.m_surface.render = false;
        }
        std::cerr<<std::flush;
        std::cerr << "\rFinished rendering: "<< std::flush;
        image.m_surface.render = false;
        image.update();
        image.display(settings);
        gui.end();
    }
    image.destroy();
    gui.destroy();

    std::cerr << "\nDone.\n";
    return 0;
}

