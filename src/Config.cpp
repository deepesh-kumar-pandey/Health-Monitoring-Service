#include "../include/Config.h"
#include <iostream>
#include <cstdlib>

std::string Config::get_encryption_key() {
    const char* env_key = std::getenv("MONITOR_KEY");
    
    if (env_key == nullptr) {
        std::cerr << "[Config] Error: MONITOR_KEY environment variable not set." << std::endl;
        std::cerr << "[Config] Please set it using: export MONITOR_KEY=\"YourSecretKey\" (Linux/Mac)" << std::endl;
        std::cerr << "[Config]                   or: $env:MONITOR_KEY=\"YourSecretKey\" (PowerShell)" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    
    return std::string(env_key);
}