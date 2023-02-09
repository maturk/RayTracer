#ifndef COLOR_H
#define COLOR_H

#include "common.h"
#include <iostream>

RAYTRACER_NAMESPACE_BEGIN

void write_color(std::ostream &out, Color pixel_color) {
    // Write the translated [0,255] value of each color component.
    // convert r,g,b to 255 range using static_cast
    // static_cast<type>
    out << static_cast<int>(255.999 * pixel_color.x()) << ' '
        << static_cast<int>(255.999 * pixel_color.y()) << ' '
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}

RAYTRACER_NAMESPACE_END
#endif