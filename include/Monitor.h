#ifndef MONITOR_H
#define MONITOR_H

#include <string>
#include <mutex>
#include <condition_variable>
#include <chrono>

class Monitor {
private:
    // --- Private Variables (State) ---
    float load_threshold;           // The CPU load limit that triggers an alert
    std::mutex mtx;                 // Mutex to protect shared resources (like the log file)
    std::condition_variable cv;     // Used for thread synchronization if we implement waiting logic
    std::string key;                // The secret XOR key for encryption
    std::string log_filename;       // Name of the file where encrypted alerts are stored

    // --- Private Helper Functions (Internal Logic) ---
    
    // Core XOR logic: Transforms plaintext to ciphertext and vice-versa
    std::string encrypt_decrypt(const std::string& data);

    // Accesses /proc/loadavg to fetch the 1-minute CPU load average
    float read_system_load();

public:
    //------------------------
    // --- Public Interface ---

    // Constructor: Initializes the threshold and filename
    // Note: 'key' should ideally be initialized here from an environment variable
    Monitor(float threshold, const std::string& log_file) 
        : load_threshold(threshold), log_filename(log_file) {}

    // Encrypts the message and writes it to the log file using a mutex lock
    void log_alert(const std::string& message);

    // The main execution loop that checks health at specific intervals
    void run_monitoring_cycle();

    // Getter function: Fixed syntax to correctly call the private helper
    float get_current_load() { 
        return read_system_load(); 
    }
}; // Added missing semicolon for class definition

#endif // Removed extra semicolon after #endif