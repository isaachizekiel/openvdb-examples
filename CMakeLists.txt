cmake_minimum_required(VERSION 3.16)

project(openvdb-examples)

# general cmake settings
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#
set(OPENVDB_BUILD_AX ON)
set(USE_AX ON)
add_subdirectory(third_party/openvdb)


find_package(OpenGL REQUIRED)

if(WIN32)
  find_package(GLEW REQUIRED)
endif()

# wraps find_package(glfw3) and sets the glfw target
add_subdirectory(third_party/glfw)

add_executable(openvdb-examples
  src/HelloWorld.cc
  src/viewer/Camera.cc
  src/viewer/ClipBox.cc
  src/viewer/Font.cc
  src/viewer/RenderModules.cc
  src/viewer/Viewer.cc
  )

#
target_link_libraries(openvdb-examples
  openvdb
  OpenGL::GL
  OpenGL::GLU
  glfw
  )

if(WIN32)
  target_link_libraries(openvdb-examples GLEW::GLEW)
endif()

target_compile_definitions(openvdb-examples PRIVATE -DGL_GLEXT_PROTOTYPES=1)


