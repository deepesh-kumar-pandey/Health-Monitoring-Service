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
    float ram_threshold;
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

    // New RAM Threshold Input
    std::cout << "[1.5/3] Enter RAM Usage % Threshold (e.g. 80.0 for 80%): ";
    while (!(std::cin >> ram_threshold) || ram_threshold < 0 || ram_threshold > 100) {
        std::cout << "Invalid input. Please enter a number between 0 and 100: ";
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

    // 5. Create Monitor instance
    Monitor sys_monitor(threshold, ram_threshold, log_file, secret_key);

    // 6. Display current system statistics before starting monitoring
    std::cout << "\n========================================\n";
    std::cout << "  CURRENT SYSTEM STATISTICS\n";
    std::cout << "========================================\n";

    // Get current system load and RAM
    float current_load = sys_monitor.get_current_load();
    float current_ram = sys_monitor.get_current_ram();

    if (current_load >= 0) {
        std::cout << "  Current CPU Load:  " << current_load << "\n";
    } else {
        std::cout << "  Current Load:      Unable to read\n";
    }

    if (current_ram >= 0) {
        std::cout << "  Current RAM Usage: " << current_ram << "%\n";
    } else {
        std::cout << "  Current RAM:       Unable to read\n";
    }

    // Get current disk status
#ifdef _WIN32
    DiskStatus ds = sys_monitor.check_disk_health("C:\\");
    std::cout << "  Disk (C:\\):       " << ds.percent_used << "% used\n";
#else
    DiskStatus ds = sys_monitor.check_disk_health("/");
    std::cout << "  Disk (/):          " << ds.percent_used << "% used\n";
#endif
    
    // Display disk space details
    std::cout << "    Total: " << (ds.total_bytes / (1024.0 * 1024.0 * 1024.0)) << " GB\n";
    std::cout << "    Free:  " << (ds.free_bytes / (1024.0 * 1024.0 * 1024.0)) << " GB\n";

    // Check database connectivity
    bool db_up = sys_monitor.check_database_health("127.0.0.1", 3306);
    std::cout << "  Database (MySQL):  " << (db_up ? "Connected" : "Not reachable") << "\n";

    // 7. Display configuration and start monitoring
    std::cout << "\n========================================\n";
    std::cout << "  MONITORING CONFIGURATION\n";
    std::cout << "========================================\n";
    std::cout << "  Target Log:       " << log_file << "\n";
    std::cout << "  CPU Threshold:    " << threshold << "\n";
    std::cout << "  RAM Threshold:    " << ram_threshold << " %\n";
    std::cout << "  Check Interval:   " << interval << " seconds\n";
    std::cout << "  Disk Alert:       > 90% usage\n";
    std::cout << "  Monitoring:       [System Load] [Disk Space] [Database]\n";
    std::cout << "  Security:         XOR-ENCRYPTION ENABLED\n";
    std::cout << "========================================\n";
    std::cout << "  Status: MONITORING ACTIVE\n";
    std::cout << "  Press Ctrl+C to stop\n";
    std::cout << "========================================\n\n";

    // 8. Start the monitoring cycle
    // This loop runs indefinitely until the process is terminated
    sys_monitor.run_monitoring_cycle(interval);

    return 0;
}