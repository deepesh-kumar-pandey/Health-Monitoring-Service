cmake_minimum_required(VERSION 3.10)
project(deepguard)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Threads library
find_package(Threads REQUIRED)

# Add source files - ADD Config.cpp HERE!
add_executable(deepguard
    src/main.cpp
    src/Monitor.cpp
    src/Config.cpp
)

# Include directories
target_include_directories(deepguard PRIVATE include)

# Link libraries
target_link_libraries(deepguard PRIVATE Threads::Threads)

# Windows-specific: Link Winsock2
if(WIN32)
    message(STATUS "Linking Winsock2 for Windows build")
    target_link_libraries(deepguard PRIVATE ws2_32)
endif()