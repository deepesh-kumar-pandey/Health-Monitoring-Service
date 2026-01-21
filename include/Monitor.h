#ifndef MONITOR_H
#define MONITOR_H

#include <string>
#include <mutex>
#include <chrono>
#include <vector>

#ifdef _WIN32
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib")
#endif

/**
 * Data structure for universal disk health
 */
struct DiskStatus {
    unsigned long long free_bytes;
    unsigned long long total_bytes;
    double percent_used;
};

/**
 * Monitor Class
 * Responsibilities:
 * 1. Tracking system CPU load (Universal).
 * 2. Encrypting alert messages using XOR logic.
 * 3. Thread-safe logging of alerts to a file.
 * 4. Checking Database connectivity.
 * 5. Monitoring Disk Space availability.
 */
class Monitor {
private:
    // --- Configuration Variables ---
    float load_threshold;        // User-defined limit (e.g., 0.75 for 75% load)
    std::string log_filename;    // The file path where logs will be stored
    
    // --- Security ---
    std::string key;             // The secret key used for XOR encryption/decryption
                                 // Stored as a private member so it's not accessible externally

    // --- Synchronization ---
    std::mutex mtx;              // Prevents multiple threads from writing to the log file 
                                 // simultaneously (avoids data corruption)

    // --- Internal Helpers ---
    
    // Applies the XOR cipher to the input string using the private 'key'
    std::string encrypt_decrypt(const std::string& data);

    // Reads and parses system load (Windows: RAM% | Linux: /proc/loadavg)
    float read_system_load();

public:
    /**
     * Constructor
     * @param threshold: CPU load limit
     * @param log_file: Destination for encrypted alerts
     * @param encryption_key: The secret key for data safety
     */
    Monitor(float threshold, const std::string& log_file, const std::string& encryption_key) 
        : load_threshold(threshold), log_filename(log_file), key(encryption_key) {}

    // Public method to manually log an encrypted alert (thread-safe)
    void log_alert(const std::string& message);

    // Starts an infinite loop that monitors load and sleeps for 'interval_seconds'
    void run_monitoring_cycle(int interval_seconds);

    // --- New Universal Health Check APIs ---

    /**
     * Checks if a database port is reachable via TCP.
     */
    bool check_database_health(const std::string& ip, int port);

    /**
     * Checks disk capacity for a given path.
     */
    DiskStatus check_disk_health(const std::string& path);

    // Inline getter to check the current system load without starting a full cycle
    float get_current_load() { 
        return read_system_load(); 
    }
};

#endif