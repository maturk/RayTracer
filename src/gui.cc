// Bring in OpenGL
// Windows
#ifdef WIN32
#include <windows.h> // Must have for Windows platform builds
#include "glee.h" // OpenGL Extension "autoloader"
#include <gl\gl.h> // Microsoft OpenGL headers
#include <gl\glu.h> // OpenGL Utilities
#include "glut.h"// Glut (Free-Glut on Windows)
#endif

// Mac OS X
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION // Defined before OpenGL and GLUT includes to avoid deprecation messages
//#include <Carbon/Carbon.h> // Brings in most Apple specific stuff
//#include <OpenGL/gl.h> // Apple OpenGL haders (version depends on OS X SDK version)
//#include <OpenGL/glu.h> // OpenGL Utilities
//#include <Glut/glut.h> // Apples Implementation of GLUT
#define Sleep(x) // Just ignore sleep on Apple
#endif

// Linux
#ifdef linux
#include <GL/gl.h>
#include <GL/glu.h>
//#include <glut.h>
#include <stdlib.h>
#endif

#include <GLFW/glfw3.h>
//#include <OpenGL/gl3.h>
//#include <OpenGL/gl3ext.h>

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
    #endif
    
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSwapInterval(1);

    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported: " << version << std::endl;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    
    glfwSetKeyCallback(window, key_callback);
    
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };  

    while (!glfwWindowShouldClose(window)){
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //ImGui_ImplGlfw_NewFrame();
        //ImGui_ImplOpenGL3_NewFrame();
        processInput(window);

        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.5f, 0.5f);
        glVertex2f(0.5f, -0.5f);
        glEnd();
        //ImGui::NewFrame();
        // render your GUI
        //ImGui::Begin("Demo window");
        //ImGui::Button("Hello!");
        //ImGui::End();

         // Render dear imgui into screen
        //ImGui::Render();
        //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glfwSwapBuffers(window);
        glfwPollEvents();

    }
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
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}