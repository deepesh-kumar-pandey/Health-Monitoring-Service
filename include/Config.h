#ifndef CONFIG_H
#define CONFIG_H

#include <string>

namespace Config {
    // Function to retrieve the encryption key safely
    std::string get_encryption_key();
}

#endif