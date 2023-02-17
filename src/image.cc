#ifndef IMAGE_H
#define IMAGE_H

#include "gui.hpp"
#include "common.hpp"
#include <iostream>
#include "stb_image.h"

/*
OpenGL object steps:
1. generate object
2. bind object
*/

class Image {
    public:
    // Surface struct to hold image data
    struct Surface {
        raytracer::Vector3f* fb; // frame buffer. Consider using a render buffer instead... 
        // (uint*)MALLOC64(w * h * sizeof(uint));
        uint* pixels;
        uint32_t width;
        uint32_t height;
        uint32_t samples;
    };

    Image():
        _surface{nullptr, 0, 0} {
            glGenFramebuffers(1 , &_fbo); // 1. generate fbo
            glGenTextures(1 , &_texture); // 1. generate texture
        }
        Surface _surface;
        GLuint _fbo; // Frame buffer object
        GLuint _texture; // Texture that attaches to frame buffer

    void updateSettings(){
        _surface.width = 1200;
        _surface.height = 900;
        _surface.samples = 10;

        // Create new texture on gpu
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo); // 2. bind fbo. GL_DRAW_FRAMEBUFFER is used as the destination for rendering, clearing and other write operations.
        glBindTexture(GL_TEXTURE_2D, _texture); // 2. bind texture. GL_TEXTURE_2D for a two-dimensional texture.
        
        // Allocate rendered image to texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB , _surface.width , _surface.height , 0, GL_RGB, GL_FLOAT, _surface.fb);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Attach texture to frame buffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture, 0);

        // Check everythig ok and bind frame buffer
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
	        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        }
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // bind to default frame buffer to show on window
    }

    void destroy()
    {
        glDeleteFramebuffers(1 , &_fbo); // Delete fbo
        glDeleteTextures(1 , &_texture); // Delete texture
    }

    Surface getSurface(){
        return _surface;
    }

    void display(){

         // Blit gpu texture to screen
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, _fbo);
        glReadBuffer(GL_COLOR_ATTACHMENT0);
        //glBlitFramebuffer(0, 0, _surface.width, _surface.height, left, top, right, bottom, GL_COLOR_BUFFER_BIT, GL_LINEAR);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    }

    void loadImage(const char* file)
        {   
            int n, width, height;
            unsigned char* data = stbi_load(file, &width, &height, &n, 0);
            if (data)
            {
                std::cout<<n<<" "<<width<<" "<<height<<std::endl;
                _surface.width = width;
                _surface.height = height;
                _surface.pixels = (uint*)malloc(width * height * sizeof(uint));
                const int s = width * height;
                if (n == 1) // greyscale
                {
                    for (int i = 0; i < s; i++)
                    {
                        const unsigned char p = data[i];
                        _surface.pixels[i] = p + (p << 8) + (p << 16);
                    }
                }
                else
                {
                    for (int i = 0; i < s; i++) _surface.pixels[i] = (data[i * n + 0] << 16) + (data[i * n + 1] << 8) + data[i * n + 2];
                }
            }
            std::cout<<*_surface.pixels<<std::endl;
            stbi_image_free(data);
        }
};

#endif