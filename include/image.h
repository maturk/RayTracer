#ifndef IMAGE_H
#define IMAGE_H

#include "common.h"
#include "gui.h"
#include "configs.h"
#include <iostream>

RAYTRACER_NAMESPACE_BEGIN

class Image {
    public: 
        struct Data {
            unsigned char* pixels; // (uint*)MALLOC64(w * h * sizeof(uint));
            int width;
            int height;
            bool render;
            bool save;
            };

        Image();
        Image(int width, int height) ;
        void updateSettings(int& width, int& height);
        void update();
        void display(const Settings& settings);
        void destroy();
        Data getSurface();
        void loadImage(const char* file);

    public:
        Data m_surface; // struct to hold data
        GLuint m_fbo; // Frame buffer object
        GLuint m_texture; // Texture that attaches to frame buffer
};

RAYTRACER_NAMESPACE_END

#endif