cmake_minimum_required(VERSION 3.14) # Minimum required version for FetchContent
project(MemoryBenchmark LANGUAGES CXX)

# Set required C++ standard (Benchmark requires C++11+)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Enforces the C++ standard
set(CMAKE_BUILD_TYPE Release) # Benchmarks should be run in Release mode

include(FetchContent) # Load the FetchContent module from CMake
# This command registers a third-party project with CMake and tells it how to find and download the source code. It only declares the intent; it doesn't perform the download yet.
FetchContent_Declare(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.9.4 # Specify the version of Google Benchmark to use
)
#[[
This is the command that executes the declaration and integrates the source code into your build.
 - Downloads
 - Adds the CMakeLists.txt of Google Benchmark to the project
 - Creates targets which are defined in CMakeLists.txt of Google Benchmark
]]#
set(BENCHMARK_DOWNLOAD_DEPENDENCIES ON CACHE BOOL "Download GTest dependency") # Ensures that Google Test is also downloaded
FetchContent_MakeAvailable(benchmark) 

# Defines the benchmark executable target
add_executable(memory_sequential_writes_and_reads memory_sequential_writes_and_reads.cpp)

# Link your executable against the necessary benchmark targets
target_link_libraries(
    memory_sequential_writes_and_reads
    PRIVATE benchmark::benchmark benchmark::benchmark_main
)