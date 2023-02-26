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


RAYTRACER_NAMESPACE_END

int main(){
    using namespace raytracer;

    // Global settings
    Settings settings;

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
                        
                        Color pixel_Color = ray_Color(r, settings.world, settings.max_depth);
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

