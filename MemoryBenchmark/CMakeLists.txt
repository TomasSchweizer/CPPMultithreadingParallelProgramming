cmake_minimum_required(VERSION 3.14) # Minimum required version for FetchContent
project(MemoryBenchmark LANGUAGES CXX)

# Set required C++ standard (Benchmark requires C++11+)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Enforces the C++ standard
set(CMAKE_BUILD_TYPE Release) # Benchmarks should be run in Release mode

