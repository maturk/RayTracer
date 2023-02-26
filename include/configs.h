#ifndef CONFIGS_H
#define CONFIGS_H

#include "hittable_list.h"
#include "worlds.h"

RAYTRACER_NAMESPACE_BEGIN

// Global Settings
struct Settings {
     float aspect_ratio = 3.0 / 2.0;
     int image_width = 800;
     int image_height = static_cast<int>(image_width / aspect_ratio);
     int samples_per_pixel = 1;
     int max_depth = 10;
     bool gamma = true;
     int num_objects = 3;
     hittable_list world = random_scene(num_objects);
};

RAYTRACER_NAMESPACE_END
#endif