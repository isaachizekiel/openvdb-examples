cmake_minimum_required(VERSION 3.16)

project(openvdb-examples)

# general cmake settings
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# opengl dependency 
find_package(OpenGL REQUIRED)

# glad extension loader library
set(GLAD_EXPORT OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/glad)

# GLFW is an Open Source, multi-platform library for OpenGL,
# OpenGL ES and Vulkan development on the *desktop*
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/glfw)

# 
add_executable(openvdb-examples src/main.cc)

target_link_libraries(openvdb-examples
  glfw glad OpenGL::GL
  )


