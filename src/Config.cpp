#include "../include/Config.h"
#include <iostream>
#include <cstdlib>

std::string Config::get_encryption_key() {
    // 1. Check the Environment first
    const char* env_key = std::getenv("MONITOR_KEY");
    
    if (env_key != nullptr) {
        return std::string(env_key);
    }

    // 2. Enforce Environment Variable
    // Start of Selection
    if (env_key == nullptr) {
        std::cerr << "[Config] Error: MONITOR_KEY environment variable not set." << std::endl;
        std::exit(EXIT_FAILURE); // Fail securely if no key is provided
    }
    
    return std::string(env_key);
}