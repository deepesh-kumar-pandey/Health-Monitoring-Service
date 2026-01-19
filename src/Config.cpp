#include "../include/Config.h"
#include<iostream>
#include<cstdlib>

std::string Config::get_encryption_key() {
    const char* key = std::getenv("MONITOR_XOR_KEY");

    if(key == nullptr) {
        std::cerr << "Error: Encryption key not set in environment variable 'MONITOR_XOR_KEY'.\n";
        exit(1);
    }
    return std::string(key);
}