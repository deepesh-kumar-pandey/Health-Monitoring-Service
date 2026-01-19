#ifndef MONITOR_H
#define MONITOR_H

#include <string>
#include <mutex>
#include <chrono>

/**
 * Monitor Class
 * Responsibilities:
 * 1. Tracking system CPU load.
 * 2. Encrypting alert messages using XOR logic.
 * 3. Thread-safe logging of alerts to a file.
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

    // Reads and parses the first value from /proc/loadavg
    float read_system_load();

public:
    /**
     * Constructor
     * @param threshold: CPU load limit
     * @param log_file: Destination for encrypted alerts
     * @param encryption_key: The secret key for data safety
     * Note: No default value for 'key' ensures the programmer consciously provides security.
     */
    Monitor(float threshold, const std::string& log_file, const std::string& encryption_key) 
        : load_threshold(threshold), log_filename(log_file), key(encryption_key) {}

    // Public method to manually log an encrypted alert (thread-safe)
    void log_alert(const std::string& message);

    // Starts an infinite loop that monitors load and sleeps for 'interval_seconds'
    void run_monitoring_cycle(int interval_seconds);

    // Inline getter to check the current system load without starting a full cycle
    float get_current_load() { 
        return read_system_load(); 
    }
};

#endif