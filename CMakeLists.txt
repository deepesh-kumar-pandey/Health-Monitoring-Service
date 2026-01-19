cmake_minimum_required(VERSION 3.10)
project(DeepGuard VERSION 1.0)

# Specify C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include the headers folder
include_directories(include)

# Find all source files in the src directory
file(GLOB SOURCES "src/*.cpp")

# Create the executable
add_executable(deepguard ${SOURCES})

# Link threading library (required for Linux)
find_package(Threads REQUIRED)
target_link_libraries(deepguard PUBLIC Threads::Threads)