cmake_minimum_required(VERSION 3.26.0)
project(natural-merge-sort)

# Set C++ standard to C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CSS_STANDARD_REQUIRED ON)

# Add include directories
include_directories(include)

# Add source files
set(SOURCES src/main.cpp src/readWrite.cpp src/sort.cpp)

# Add executable target
add_executable(natural-merge-sort ${SOURCES})