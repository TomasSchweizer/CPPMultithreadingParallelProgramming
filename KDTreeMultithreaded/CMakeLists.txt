cmake_minimum_required(VERSION 3.15)
project(KDTreeMultithreaded VERSION 0.1.0 LANGUAGES CXX)

# Set C++ standard and disable compiler-specific extensions
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenMP REQUIRED)
if(OpenMP_CXX_FOUND)
    message(STATUS "OpenMP found")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
else()
    message(FATAL_ERROR "OpenMP not found")
endif()

# This allows all cpp to include headers from the include directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# Ceates the KDTree library target
add_library(KDTree STATIC
    src/KDTree.cpp
    )

target_include_directories(KDTree PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    )
target_link_libraries(KDTree PUBLIC OpenMP::OpenMP_CXX)

add_executable(demo_kdtree
    examples/demo_kdtree.cpp
    )
target_link_libraries(demo_kdtree PRIVATE KDTree)
