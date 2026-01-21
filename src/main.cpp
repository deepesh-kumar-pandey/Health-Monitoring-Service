#include "../include/Monitor.h"
#include "../include/Config.h"
#include <iostream>
#include <string>
#include <limits>

/**
 * DEEP GUARD - Main Entry Point
 * Handles user configuration and initializes the monitoring engine.
 */
int main() {
    // 1. Fetch the secret key safely via the Config module (Environment variable)
    // This keeps the actual password out of your source code for safety.
    std::string secret_key = Config::get_encryption_key();
    
    // Check if key exists before starting
    if (secret_key.empty()) {
        std::cerr << "CRITICAL ERROR: Environment variable MONITOR_KEY is not set.\n";
        return 1;
    }
    
    float threshold;
    std::string log_file;
    int interval;

    std::cout << "-------------------------------------------\n";
    std::cout << "      DEEP GUARD: UNIVERSAL SETUP          \n";
    std::cout << "-------------------------------------------\n";

    // 2. Interactive Input: Load Threshold
    // NOTE: On Windows, this represents RAM Usage % (0-100). 
    // On Linux, this represents CPU Load Average (e.g., 0.75).
#ifdef _WIN32
    std::cout << "[1/3] Enter RAM Usage % Threshold (e.g. 80.0 for 80%): ";
#else
    std::cout << "[1/3] Enter CPU Load Threshold (e.g. 0.75 for 75%): ";
#endif

    while (!(std::cin >> threshold) || threshold < 0) { 
        std::cout << "Invalid input. Please enter a positive number: ";
        std::cin.clear(); 
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // 3. Interactive Input: Log Filename
    std::cout << "[2/3] Enter name for the log file (e.g., alerts.log): ";
    std::cin >> log_file;

    // 4. Interactive Input: Frequency
    std::cout << "[3/3] Enter check interval in seconds (e.g., 5): ";
    while(!(std::cin >> interval) || interval <= 0) {
        std::cout << "Invalid input. Please enter a positive integer: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // 5. Initialization
    // We pass the secret_key here so the Monitor can encrypt logs safely.
    Monitor sys_monitor(threshold, log_file, secret_key);

    std::cout << "\n========================================\n";
    std::cout << "  Configuration Saved. Monitoring Started.\n";
    std::cout << "  Target Log: " << log_file << "\n";
    std::cout << "  Threshold:  " << threshold << "\n";
    std::cout << "  Interval:   " << interval << "s\n";
    std::cout << "  Monitoring: [System Load] [Disk Space] [Database]\n";
    std::cout << "  Security:   XOR-ENCRYPTION ENABLED\n";
    std::cout << "========================================\n";

    // 6. Start the cycle using the user's chosen interval
    // This loop runs indefinitely until the process is terminated.
    sys_monitor.run_monitoring_cycle(interval);

    return 0;
}