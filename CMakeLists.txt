cmake_minimum_required(VERSION 3.10)
project(DeepGuard VERSION 1.0)

# Specify C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include the headers folder
include_directories(include)

# Explicitly list source files (Better practice than GLOB for build tracking)
set(SOURCES 
    "src/main.cpp" 
    "src/Monitor.cpp"
)

# Create the executable
add_executable(deepguard ${SOURCES})

# --- LINKING LIBRARIES ---

# 1. Threading library (Required for std::thread on Linux)
find_package(Threads REQUIRED)
target_link_libraries(deepguard PUBLIC Threads::Threads)

# 2. Windows Sockets (Required for check_database_health on Windows)
if(WIN32)
    target_link_libraries(deepguard PUBLIC ws2_32)
    message(STATUS "Linking Winsock2 for Windows build")
endif()