#ifndef IMAGE_H
#define IMAGE_H

#include "gui.h"
#include "common.h"
#include <iostream>
#include "stb_image.h"

RAYTRACER_NAMESPACE_BEGIN
class Image {

    public:
        // struct to hold image data
        struct Data {
            unsigned char* pixels; // (uint*)MALLOC64(w * h * sizeof(uint));
            int width;
            int height;
            bool render;
        };

    Image(): m_surface{nullptr, 0, 0} {
        glGenFramebuffers(1 , &m_fbo); // generate fbo
        glGenTextures(1 , &m_texture); // generate texture
    }
    
    Image(int width, int height): m_surface{nullptr, width, height} {
        glGenFramebuffers(1 , &m_fbo); // generate fbo
        glGenTextures(1 , &m_texture); // generate texture
        m_surface.pixels = (unsigned char *)malloc(4 * width * height);
        m_surface.render = true;
    }

    void updateSettings(int& width, int& height){
        m_surface.height = height;
        m_surface.width = width;
        free(m_surface.pixels);
        m_surface.pixels = (unsigned char *)malloc(4 * width * height);
        for (int y = 0; y <height; y++){
            for (int x = 0; x < width; x++){
                m_surface.pixels[(y*width +x) * 4 +0] = 0.0 ;
                m_surface.pixels[(y*width +x) * 4 +1] = 0.0 ;
                m_surface.pixels[(y*width +x) * 4 +2] = 0.0 ;
                m_surface.pixels[(y*width +x) * 4 +3] = 0.0 ;
            }
        }
        m_surface.render = true;
        update();
    }
        
    void update(){
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo); // Bind draw frame buffer and texture
        glBindTexture(GL_TEXTURE_2D, m_texture); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA , m_surface.width , m_surface.height , 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)m_surface.pixels); // Allocate rendered image to 2D texture
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0); // Attach texture to draw frame buffer
        
        // Check everythig ok and bind draw frame buffer
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
	        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        }
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // bind to default frame buffer to show on window
    }

    void display(){        
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
        glReadBuffer(GL_COLOR_ATTACHMENT0);
        glBlitFramebuffer(0, 0, m_surface.width, m_surface.height, 0, 0, m_surface.width, m_surface.height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    }

    void destroy()
    {
        glDeleteFramebuffers(1 , &m_fbo); // Delete fbo
        glDeleteTextures(1 , &m_texture); // Delete texture
    }

    Data getSurface(){
        return m_surface;
    }

    void loadImage(const char* file){   
            stbi_set_flip_vertically_on_load(true); // flip image for OpenGL coordinate system
            int width, height, n;
            m_surface.pixels = stbi_load(file, &width, &height, &n, 0);
            m_surface.width = width;
            m_surface.height = height;
        }

    public:
        Data m_surface; // struct to hold data
        GLuint m_fbo; // Frame buffer object
        GLuint m_texture; // Texture that attaches to frame buffer
};
RAYTRACER_NAMESPACE_END
#endif