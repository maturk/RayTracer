# This is the main CMake build file used to complile my RayTracer program
cmake_minimum_required(VERSION 3.13.4)
# Set the C++ standard whose features are requested to build this target. C++11 required for dependencies ImGui 
set(CMAKE_CXX_STANDARD 17)

# Project
project(RayTracer)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# OpenMP parallelisation
if(APPLE)
    set(CMAKE_C_COMPILER "/usr/bin/clang")
    set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

    if(CMAKE_C_COMPILER_ID MATCHES "Clang\$")
        set(OpenMP_C_FLAGS "-Xpreprocessor -fopenmp")
        set(OpenMP_C_LIB_NAMES "omp")
        set(OpenMP_omp_LIBRARY omp)
    endif()
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang\$")
        set(OpenMP_CXX_FLAGS "-Xclang -fopenmp")
        set(OpenMP_CXX_LIB_NAMES "omp")
        set(OpenMP_omp_LIBRARY omp)
    endif()
endif()
find_package(OpenMP REQUIRED)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

# Eigen 
# Manually set Eigen paths to local Eigen folder
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/ext/eigen/cmake/)
set(EIGEN3_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext/eigen)
find_package (Eigen3 3.3 REQUIRED)
if (APPLE)
  # Work around MacOS CI errors saying:
  # Modules/ThirdParty/Eigen3/src/itkeigen/…/itkeigen/Eigen/Core:70:10: fatal error: ‘omp.h’ file not found
  # add_compile_definitions(EIGEN_DONT_PARALLELIZE )
endif()

# Include exteral libraries
include_directories(ext/ include/ SYSTEM ${EIGEN3_INCLUDE_DIR} "/opt/homebrew/Cellar/libomp/15.0.7/include")

add_subdirectory(ext/glfw)
add_subdirectory(ext/glad)
add_subdirectory(ext/glew-cmake)

# Main program
file(GLOB SOURCES "src/*.cpp" "src/*.h" "include/*.h")
add_executable(${PROJECT_NAME} ${SOURCES})

# ImGui
set(IMGUI_PATH  "ext/imgui")

# Compile ImGui as static library 
file(GLOB IMGUI_SOURCES ${IMGUI_PATH}/*.cpp ${IMGUI_PATH}/*.h ${IMGUI_PATH}/backends/*opengl3*.cpp ${IMGUI_PATH}/backends/*opengl3*.h ${IMGUI_PATH}/backends/*glfw*.cpp ${IMGUI_PATH}/backends/*glfw*.h $ext/glfw/include/*)  
add_library("ImGui" STATIC ${IMGUI_SOURCES})
target_include_directories("ImGui" PUBLIC ${IMGUI_PATH} ${IMGUI_PATH}/backends  ext/glfw/include  ext/glfw/deps PRIVATE ext/glfw/src 
)
# Link Targets
target_compile_options(${PROJECT_NAME} PRIVATE "${OpenMP_CXX_FLAGS}")
target_link_libraries(${PROJECT_NAME} PUBLIC ImGui glfw glad ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} libglew_static 
    PRIVATE OpenMP::OpenMP_CXX
)