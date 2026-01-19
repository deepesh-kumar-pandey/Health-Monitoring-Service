#include "../include/Monitor.h"
#include "../include/Config.h"
#include <iostream>
#include <string>
#include <limits>

int main() {
    // 1. Fetch the secret key safely via the Config module (Environment variable)
    // This keeps the actual password out of your source code for safety.
    std::string secret_key = Config::get_encryption_key();
    
    float threshold;
    std::string log_file;
    int interval;

    std::cout << "-------------------------------------------\n";
    std::cout << "      DEEP GUARD: SYSTEM SETUP         \n";
    std::cout << "-------------------------------------------\n";

    // 2. Interactive Input: Load Threshold
    std::cout << "[1/3] Enter CPU Load Threshold (e.g. 0.75 for 75%): ";
    while (!(std::cin >> threshold) || threshold < 0 || threshold > 5.0) { // Allowed > 1.0 for multi-core systems
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
    std::cout << "  Target: " << log_file << " | Alert at: >" << threshold << "\n";
    std::cout << "  Interval: " << interval << "s | Security: ENABLED\n";
    std::cout << "========================================\n";

    // 6. Start the cycle using the user's chosen interval
    sys_monitor.run_monitoring_cycle(interval);

    return 0;
}