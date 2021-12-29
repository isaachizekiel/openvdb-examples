cmake_minimum_required(VERSION 3.16)

project(openvdb-examples)

# general cmake settings
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


#
add_subdirectory(third_party/openvdb)

# 
add_executable(openvdb-examples src/main.cc)

#
target_link_libraries(openvdb-examples openvdb_static)


