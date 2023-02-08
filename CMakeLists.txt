# This is the main CMake build file used to complile my RayTracer program
cmake_minimum_required(VERSION 3.2)

# Set the C++ standard whose features are requested to build this target. C++11 required for dependencies ImGui 
set(CMAKE_CXX_STANDARD 11)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/ext/eigen/cmake/)
set(EIGEN3_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext/eigen)
find_package (Eigen3 3.3 REQUIRED)
project(RayTracer)

# Include exteral libraries
include_directories(ext/
SYSTEM ${EIGEN3_INCLUDE_DIR})

# Main program

file(GLOB SOURCES "src/*.cpp" "src/*.h")

add_executable(${PROJECT_NAME} ${SOURCES})


# ImGui
# Set ImGui file path
set(IMGUI_PATH  "ext/imgui")

# Compile ImGui as static library 
file(GLOB IMGUI_SOURCES ${IMGUI_PATH}/*.cpp ${IMGUI_PATH}/*.h ${IMGUI_PATH}/backends/*opengl3*.cpp ${IMGUI_PATH}/backends/*opengl3*.h ${IMGUI_PATH}/backends/*glfw*.cpp ${IMGUI_PATH}/backends/*glfw*.h $ext/glfw/include/*)  
add_library("ImGui" STATIC ${IMGUI_SOURCES})
target_include_directories("ImGui" PUBLIC ${IMGUI_PATH} ${IMGUI_PATH}/backends  ext/glfw/include  ext/glfw/deps PRIVATE ext/glfw/src 
)

list(APPEND NGP_INCLUDE_DIRECTORIES
	ImGui
)

# Link Targets
target_link_libraries(${PROJECT_NAME} PUBLIC ImGui 
)