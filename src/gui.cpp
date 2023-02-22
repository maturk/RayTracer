#include "gui.h"
#include "image.cpp"

#include <GLFW/glfw3.h>

#include <iostream>
#include <ostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

RAYTRACER_NAMESPACE_BEGIN

class GUI {
    public:
        GUI(int width, int height): m_width(width), m_height(height) {
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

            m_window = glfwCreateWindow(m_width, m_height, "Renderer", NULL, NULL);
            glfwMakeContextCurrent(m_window);
            glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

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
            ImGui_ImplGlfw_InitForOpenGL(m_window, true);
            ImGui_ImplOpenGL3_Init(glsl_version);
            // Setup Dear ImGui style
            ImGui::StyleColorsDark();
            ImGui_ImplOpenGL3_NewFrame();
            
            glfwSetKeyCallback(m_window, key_callback);

            if (!m_window){
                glfwTerminate();
                exit(EXIT_FAILURE);
            }
            GLenum err = glewInit();
        }

        bool open(){
            return !glfwWindowShouldClose(m_window);
        }

        void start(Image& image){
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // render ImGUI
            ImGui_ImplGlfw_NewFrame();
            ImGui_ImplOpenGL3_NewFrame();

            ImGui::NewFrame();  
            if (ImGui::Begin("Controls"));
            if (ImGui::Button("Save"))
                {
                    
                }
            ImGui::End();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            
            processInput(m_window);

            // Update viewport width height if user changes display size
            int width, height;
            glfwGetWindowSize(m_window, &width, &height);
            if (width != image.m_surface.width | height != image.m_surface.height){
                image.updateSettings(width, height);
            }
        }

        void end(){
            // render ImGUI
            ImGui_ImplGlfw_NewFrame();
            ImGui_ImplOpenGL3_NewFrame();

            ImGui::NewFrame();  
            if (ImGui::Begin("Controls"));
            if (ImGui::Button("Save"))
                {
                    
                }
            ImGui::End();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(m_window);
            glfwPollEvents();
        }

        void destroy(){
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
            glfwTerminate();
        }

        // glfw: whenever the window size changed (by OS or user resize) this callback function executes
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
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

        // Error call back
        static void error_callback(int error, const char* description)
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

    public:
        GLFWwindow* m_window;
        int m_width;
        int m_height;
};


RAYTRACER_NAMESPACE_END