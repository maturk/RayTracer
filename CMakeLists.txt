# This is the main CMake build file used to complile my RayTracer program
cmake_minimum_required(VERSION 3.13.4)
# Set the C++ standard whose features are requested to build this target. C++11 required for dependencies ImGui 
set(CMAKE_CXX_STANDARD 11)

# Eigen 
# Manually set Eigen paths to local Eigen folder
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/ext/eigen/cmake/)
set(EIGEN3_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext/eigen)
find_package (Eigen3 3.3 REQUIRED)

# Project
project(RayTracer)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Include exteral libraries
include_directories(ext/ include/ SYSTEM ${EIGEN3_INCLUDE_DIR})

add_subdirectory(ext/glfw)
add_subdirectory(ext/glad)
add_subdirectory(ext/glew-cmake)

# Main program
file(GLOB SOURCES "src/*.cpp" "src/*.h" "include/*")
add_executable(${PROJECT_NAME} ${SOURCES})

# Scratch progam
file(GLOB GUI_SOURCES "src/*.cc" "src/*.hpp" "include/*")
add_executable("gui" ${GUI_SOURCES} )

# ImGui
set(IMGUI_PATH  "ext/imgui")

# Compile ImGui as static library 
file(GLOB IMGUI_SOURCES ${IMGUI_PATH}/*.cpp ${IMGUI_PATH}/*.h ${IMGUI_PATH}/backends/*opengl3*.cpp ${IMGUI_PATH}/backends/*opengl3*.h ${IMGUI_PATH}/backends/*glfw*.cpp ${IMGUI_PATH}/backends/*glfw*.h $ext/glfw/include/*)  
add_library("ImGui" STATIC ${IMGUI_SOURCES})
target_include_directories("ImGui" PUBLIC ${IMGUI_PATH} ${IMGUI_PATH}/backends  ext/glfw/include  ext/glfw/deps PRIVATE ext/glfw/src 
)

# Link Targets
target_link_libraries(${PROJECT_NAME} PUBLIC ImGui glfw glad ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} libglew_static
)

target_link_libraries("gui" PUBLIC glfw ImGui glad ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} libglew_static
)