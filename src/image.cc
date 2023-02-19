#ifndef IMAGE_H
#define IMAGE_H

#include "gui.hpp"
#include "common.hpp"
#include <iostream>
#include "stb_image.h"


class Image {

    public:
        // struct to hold image data
        struct Data {
            unsigned char* pixels; // (uint*)MALLOC64(w * h * sizeof(uint));
            int width;
            int height;
        };

    Image(): m_surface{nullptr, 0, 0} {
        glGenFramebuffers(1 , &m_fbo); // generate fbo
        glGenTextures(1 , &m_texture); // generate texture
        }
    
    Image(int w, int h): m_surface{nullptr, w, h} {
        glGenFramebuffers(1 , &m_fbo); // generate fbo
        glGenTextures(1 , &m_texture); // generate texture
        //std::cout<<m_surface.width<<" "<<m_surface.height<<std::endl;
        m_surface.pixels = (unsigned char *)malloc(3 * w * h);
        }

    void updateSettings(int& width, int& height){
        m_surface.height = height;
        m_surface.width = width;
        free(m_surface.pixels);
        m_surface.pixels = new unsigned char[width * height * 3];
        std::cerr<<"Aspect Ratio: "<<(float)(m_surface.width)/(float)(m_surface.height)<<std::endl;
        std::cerr<<"Window width : "<<m_surface.width<<" height "<<m_surface.height<<std::endl;
        //glDeleteTextures(1, &m_texture);
        //glBindTexture(GL_TEXTURE_2D, m_texture); 
        //m_surface.render = true;
        //update();
    }
    void update(){
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo); // Bind draw frame buffer and texture
        glBindTexture(GL_TEXTURE_2D, m_texture); 
        for (int y = 0; y <m_surface.height; y++){
            for (int x = 0; x < m_surface.width; x++){
                //std::cout<<m_surface.pixels[(y*m_surface.width +x) * n +0]<<std::endl;
                m_surface.pixels[(y*m_surface.width +x) * 3 +0] = 120;
                m_surface.pixels[(y*m_surface.width +x) * 3 +1] = 120;
                m_surface.pixels[(y*m_surface.width +x) * 3 +2] = 10;
            }
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB , m_surface.width , m_surface.height , 0, GL_RGB, GL_UNSIGNED_BYTE, m_surface.pixels); // Allocate rendered image to 2D texture
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
        glBlitFramebuffer(0, 0, m_surface.width, m_surface.height, 0, 0, m_surface.width, m_surface.height, GL_COLOR_BUFFER_BIT, GL_LINEAR); // copies from read to draw fb (default)
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
            //m_surface.pixels = stbi_load(file, &width, &height, &n, 0);
            //std::cout <<sizeof(m_surface.pixels)<<std::endl;
            m_surface.pixels = (unsigned char*)malloc(3 * 1200 * 900);
            m_surface.width = width;
            m_surface.height = height;
            for (int y = 0; y <900; y++){
                for (int x = 0; x < 1200; x++){
                    //std::cout<<m_surface.pixels[(y*m_surface.width +x) * n +0]<<std::endl;
                    m_surface.pixels[(y*m_surface.width +x) * n +0] = 120;
                    m_surface.pixels[(y*m_surface.width +x) * n +1] = 120;
                    m_surface.pixels[(y*m_surface.width +x) * n +2] = 10;
                }
            }
        }

    public:
        Data m_surface; // struct to hold data
        GLuint m_fbo; // Frame buffer object
        GLuint m_texture; // Texture that attaches to frame buffer
};

#endif