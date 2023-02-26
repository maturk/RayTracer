#ifndef WORLDS_H
#define WORLDS_H

#include "common.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

RAYTRACER_NAMESPACE_BEGIN

inline hittable_list random_scene(int num_objects) {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(Color(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(Point3f(0,-1000,0), 1000, ground_material));
    
    if (num_objects>1){
        auto material1 = make_shared<dielectric>(1.5);
        world.add(make_shared<Sphere>(Point3f(0, 1, 0), 1.0, material1));
    }
    if (num_objects>2){
        auto material2 = make_shared<lambertian>(Color(0.4, 0.2, 0.1));
        world.add(make_shared<Sphere>(Point3f(-4, 1, 0), 1.0, material2));
    }
    if (num_objects>0){
        auto material3 = make_shared<metal>(Color(0.7, 0.6, 0.5), 0.0);
        world.add(make_shared<Sphere>(Point3f(4, 1, 0), 1.0, material3));
    }
    if (num_objects > 3) {
        for (int i = 0; i < num_objects - 3; i++ ) {
            int a = static_cast<int>(11*random(-1,1));
            int b = static_cast<int>(11*random(-1,1));

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
    return world;
};

inline hittable_list sphere_world() {
    hittable_list world;
    auto material_ground = make_shared<lambertian>(Color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(Color(0.1, 0.2, 0.5));
    auto material_left   = make_shared<dielectric>(1.5);
    auto material_right  = make_shared<metal>(Color(0.8, 0.6, 0.2), 0.0);
    world.add(make_shared<Sphere>(Point3f( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<Sphere>(Point3f( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(make_shared<Sphere>(Point3f(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<Sphere>(Point3f(-1.0,    0.0, -1.0),  -0.4, material_left));
    world.add(make_shared<Sphere>(Point3f( 1.0,    0.0, -1.0),   0.5, material_right));
    return world;
}

RAYTRACER_NAMESPACE_END
#endif