#include "gui.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "image.cc"

#include <GLFW/glfw3.h>

#include <iostream>
#include <ostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// Define glsl_version
#if defined(IMGUI_IMPL_OPENGL_ES2)
        const char* glsl_version = "#version 100";
#elif defined(IMGUI_IMPL_OPENGL_ES3)
        const char* glsl_version = "#version 300 es";
#elif defined(__APPLE__)
        const char* glsl_version = "#version 150";
#else
        const char* glsl_version = "#version 130";
#endif

// Some helpful functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// Error call back
void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

// Static variables are not destoryed out of scope. When a function inside a class is declared as static, 
// it can be accessed outside the class using the class name and scope resolution operator (::), without creating any object.
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

// Shaders
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

int main(){
    GLFWwindow* window;
 
    glfwSetErrorCallback(error_callback);
    
    if (!glfwInit())
        exit(EXIT_FAILURE);
    
    // Minimum target is OpenGL 4.1
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
    #endif
    
    window = glfwCreateWindow(1200, 900, "Simple example", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSwapInterval(1); //vSync

    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported: " << version << std::endl;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    // Setup ImGUI Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    
    ImGui_ImplOpenGL3_NewFrame();
    
    glfwSetKeyCallback(window, key_callback);
    
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    GLenum err = glewInit();

    Image image;
    const char * file = "/Users/maturk/Downloads/test.jpg";
    //image.loadImage(file);
    //image.getSurface();

    //fbo
    unsigned int fbo;
    glGenFramebuffers(1, &fbo);
    //glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);  

    //texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load(file, &width, &height, &nrChannels, 0); 
    unsigned int texture;
    glGenTextures(1, &texture);  
    glBindTexture(GL_TEXTURE_2D, texture);  
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    
    std::cout<<width<<" "<<height <<" hi" <<std::endl;

    // attach texture to frame buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0); 
    //check for errors 
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);  


    while (!glfwWindowShouldClose(window)){
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        processInput(window);

        //render
         //update texture
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB , 1200 , 900 , 0, GL_RGB, GL_FLOAT, data);
        //glBlitFramebuffer(0, 0, 1200, 900,
        //          0, 0, 1200, 900,
        //          GL_COLOR_BUFFER_BIT, GL_LINEAR);
        //glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // update
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo); //draw
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1200, 900, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);//draw
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);// draw frame buffer bound to default fb 

        //display
        //glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo); //read
        glReadBuffer(GL_COLOR_ATTACHMENT0);
        glBlitFramebuffer(0, 0, 1200, 900, 0, 0, 1200, 900, GL_COLOR_BUFFER_BIT, GL_LINEAR); // copies from read to draw fb (default)
        //glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

        // render your GUI
        ImGui::NewFrame();  
        ImGui::Begin("Menu");
        ImGui::Text("Loading frame buffer as image");
        ImGui::End();
        
        // Render dear imgui into screen
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    glDeleteFramebuffers(1, &fbo);  
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}