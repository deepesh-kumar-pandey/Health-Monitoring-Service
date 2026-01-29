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
 * Notification severity levels
 */
enum class NotificationLevel {
    INFO,       // Informational messages
    WARNING,    // Warning messages (yellow)
    CRITICAL    // Critical alerts (red)
};

/**
 * Monitor Class
 * Responsibilities:
 * 1. Tracking system CPU load (Universal).
 * 2. Encrypting alert messages using XOR logic.
 * 3. Thread-safe logging of alerts to a file.
 * 4. Checking Database connectivity.
 * 5. Monitoring Disk Space availability.
 * 6. Sending system notifications.
 */
class Monitor {
private:
    // --- Configuration Variables ---
    float load_threshold;        // User-defined limit (e.g., 0.75 for 75% load)
    float ram_threshold;         // User-defined RAM limit (e.g., 80.0 for 80%)
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

    // Reads and parses RAM usage percentage (Linux: /proc/meminfo)
    float read_ram_usage();

public:
    /**
     * Constructor
     * @param threshold: CPU load limit
     * @param log_file: Destination for encrypted alerts
     * @param encryption_key: The secret key for data safety
     */
    Monitor(float threshold, float ram_limit, const std::string& log_file, const std::string& encryption_key) 
        : load_threshold(threshold), ram_threshold(ram_limit), log_filename(log_file), key(encryption_key) {}

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

    // Inline getter to check the current RAM usage
    float get_current_ram() {
        return read_ram_usage();
    }

    /**
     * Sends a native system notification/toast.
     * @param title: The notification title
     * @param message: The notification body
     * @param level: Severity level (INFO, WARNING, CRITICAL)
     * @return true if notification sent successfully
     */
    bool send_system_notification(
        const std::string& title, 
        const std::string& message,
        NotificationLevel level = NotificationLevel::INFO
    );
};

#endif