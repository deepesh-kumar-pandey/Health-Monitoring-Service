#include "../include/Monitor.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <vector>

/**
 * Reads the 1-minute CPU load average from the Linux kernel.
 * Accesses the virtual filesystem /proc/loadavg.
 */
float Monitor::read_system_load() {
    std::ifstream file("/proc/loadavg");
    float load = 0.0f;

    // Return -1.0f as a sentinel value if file access fails
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open /proc/loadavg." << std::endl;
        return -1.0f;
    }

    // Parse the first float value from the file
    if (file >> load) {
        file.close();
    } else {
        std::cerr << "Error: Failed to parse load data." << std::endl;
        file.close();
        return -1.0f;
    }

    return load;
}

/**
 * XOR Encryption/Decryption Logic.
 * Same loop toggles between plain and scrambled states.
 */
std::string Monitor::encrypt_decrypt(const std::string& data) {
    std::string result = data;
    for(size_t i = 0; i < data.size(); ++i) {
        // Modulo (%) wraps the key around if the data is longer than the key
        result[i] = data[i] ^ key[i % key.size()];
    }
    return result;
}

/**
 * Thread-safe logging.
 * Protects the file from race conditions using a Mutex.
 */
void Monitor::log_alert(const std::string& message) {
    // std::lock_guard locks 'mtx' and unlocks it automatically when function ends
    std::lock_guard<std::mutex> lock(mtx); 

    // Scramble message before opening the file
    std::string encrypted = encrypt_decrypt(message);

    // Open in 'app' (append) mode to keep historical data
    std::ofstream log_file(log_filename, std::ios::app);
    
    if(log_file.is_open()) {
        log_file << encrypted << "\n";
        log_file.close();
    } else {
        std::cerr << "Error: Unable to open log file: " << log_filename << "\n";
    }
}

/**
 * Infinite monitoring loop.
 * Checks system health every 5 seconds.
 */
void Monitor::run_monitoring_cycle() {
    while(true) {
        // Note: Changed get_current_load() to read_system_load() to match function name above
        float current_load = read_system_load();
        
        if(current_load < 0) {
            // Wait and retry if a system read error occurs
            std::this_thread::sleep_for(std::chrono::seconds(5));
            continue;
        }

        if(current_load > load_threshold) {
            std::string alert_message = "Alert: High CPU Load Detected - " + std::to_string(current_load);
            log_alert(alert_message);
            std::cout << "[Monitor] Alert logged to " << log_filename << "\n";
        } else {
            std::cout << "[Monitor] System Healthy. Load: " << current_load << "\n";
        }

        // Pause for 5 seconds between checks to save CPU resources
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}