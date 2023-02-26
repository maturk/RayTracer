#ifndef COLOR_H
#define COLOR_H

#include "common.h"
#include <iostream>
#include <fstream>
#include "image.h"
#include <filesystem>

RAYTRACER_NAMESPACE_BEGIN

void save_image(Image image) {
    std::string cwd = std::filesystem::current_path().string();
    std::filesystem::path out_folder (cwd+"/out/");
    if (!std::filesystem::exists(out_folder)) { // Check if src folder exists
        std::filesystem::create_directory(out_folder); // create src folder

    }
    std::ofstream out(cwd+"/out/image.ppm");
    out << "P3\n" << image.m_surface.width << " " << image.m_surface.height << "\n255\n";
    for (int y = 0; y < image.m_surface.height; y++){
        for (int x = 0; x < image.m_surface.width; x++){
            auto r = image.m_surface.pixels[(y*image.m_surface.width +x) * 4 +0];
            auto g = image.m_surface.pixels[(y*image.m_surface.width +x) * 4 +1];
            auto b = image.m_surface.pixels[(y*image.m_surface.width +x) * 4 +2];

            // Write the translated [0,255] value of each color component.
            // convert r,g,b to 255 range using static_cast
            // static_cast<type>
            out << static_cast<int>(clamp(r, 0.0, 255)) << " "
                << static_cast<int>(clamp(g, 0.0, 255)) << " "
                << static_cast<int>(clamp(b, 0.0, 255)) << "\n";
        }
    }
    out.close();

    std::cout<<"\nOutput file saved to: "<<cwd+"/out/image.ppm"<<std::endl;
}

RAYTRACER_NAMESPACE_END
#endif