#include "../include/Config.h"
#include <iostream>
#include <cstdlib>

std::string Config::get_encryption_key() {
    // 1. Check the Environment first
    const char* env_key = std::getenv("MONITOR_KEY");
    
    if (env_key != nullptr) {
        return std::string(env_key);
    }

    // 2. Fallback: Use a default developer key
    // This allows the program to run even if you didn't 'export' anything
    std::string default_key = "DEV_DEBUG_KEY_99"; 
    
    std::cout << "[Config] No MONITOR_KEY found in environment. Using default dev key." << std::endl;
    return default_key;
}