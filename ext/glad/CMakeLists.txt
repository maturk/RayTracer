cmake_minimum_required(VERSION 3.13.4)
project(Glad)

add_library(glad ${CMAKE_CURRENT_SOURCE_DIR}/include/glad/glad.h src/glad.c)
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)