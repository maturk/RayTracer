#include "gui.h"
#include "image.h"

#include <GLFW/glfw3.h>

#include <iostream>
#include <ostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

RAYTRACER_NAMESPACE_BEGIN
const float float2InputWidth = 121.f;

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
            
            if (!m_window){
                glfwTerminate();
                exit(EXIT_FAILURE);
            }
            GLenum err = glewInit();
        }

        bool open(){
            return !glfwWindowShouldClose(m_window);
        }

        void start(Image& image, Settings& settings){
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            processInput(m_window);

            // render ImGUI
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::Begin("Renderer");
            if (ImGui::CollapsingHeader("Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::SliderFloat("Aspect ratio", &settings.aspect_ratio, 1, 2);
                ImGui::SliderInt("Height", &image.m_surface.height, 1, 1080);
                ImGui::SliderInt("Width", &image.m_surface.width, 1, 1920);
                ImGui::SliderInt("Samples", &settings.samples_per_pixel, 1, 50); 
                ImGui::SliderInt("Max depth", &settings.max_depth, 1, 20);  
                ImGui::SliderInt("Num of random spheres", &settings.num_objects, 1, 40);
                settings.world = random_scene(settings.num_objects);
            }
            ImGui::Checkbox("Gamma correction", &settings.gamma); 
            ImGui::Text("Render image with new settings: "); 
            ImGui::SameLine();
            if (ImGui::Button("Render")){
                    image.m_surface.render = true;
            }
            ImGui::Text("Save image to ./out/ folder: "); 
            ImGui::SameLine();
            if (ImGui::Button("Save")){
                    image.m_surface.save = true;
            }
            if (ImGui::Button("Stop")){
                    image.m_surface.render = false;
            }
            
            ImGui::End();
            ImGui::Render();

            // Update viewport width height if user changes display size
            int width, height;
            glfwGetWindowSize(m_window, &width, &height);
            if (width != image.m_surface.width | height != image.m_surface.height){
                //image.updateSettings(width, height);
                settings.image_height = height;
                settings.image_width = width;
            }
        }

        void end(){
            // render ImGUI
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

    public:
        GLFWwindow* m_window;
        int m_width;
        int m_height;
};


RAYTRACER_NAMESPACE_END