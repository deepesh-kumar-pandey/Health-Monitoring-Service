#include "../include/Monitor.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <vector>

/** * --- OS-SPECIFIC INCLUDES ---
 * We use conditional compilation to include the correct system APIs
 * for Windows (MSVC/MinGW) and Unix-based systems (Linux/macOS).
 */
#ifdef _WIN32
    #include <windows.h>   // For Memory and Disk APIs
    #include <winsock2.h>  // For Network Sockets
    #include <winuser.h>   // For MessageBox notifications
    #pragma comment(lib, "ws2_32.lib") // Links the Winsock library on Windows
#else
    #include <unistd.h>      // Standard symbolic constants and types
    #include <sys/statvfs.h> // For Linux disk space stats
    #include <sys/socket.h>  // For Linux network sockets
    #include <netinet/in.h>  // For IP address structures
    #include <arpa/inet.h>   // For IP address conversion
    #include <cstdlib>       // For system() command
#endif

/**
 * @brief Reads the current system workload.
 * * Windows: Retrieves the percentage of physical memory currently in use.
 * Linux: Parses the first float from /proc/loadavg (1-minute CPU load average).
 * * @return float: Percentage (0-100) or Load Average. Returns -1.0 on failure.
 */
float Monitor::read_system_load() {
#ifdef _WIN32
    // Windows API to get system-wide memory status
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    if (GlobalMemoryStatusEx(&memInfo)) {
        return static_cast<float>(memInfo.dwMemoryLoad);
    }
    return -1.0f;
#else
    // Linux virtual file system read
    std::ifstream file("/proc/loadavg");
    float load = 0.0f;
    if (!file.is_open()) return -1.0f;
    if (file >> load) {
        file.close();
        return load;
    }
    return -1.0f;
#endif
}

/**
 * @brief Checks disk capacity and calculates usage percentage.
 * * @param path: The directory or drive to check (e.g., "C:\\" or "/").
 * @return DiskStatus: A struct containing free bytes, total bytes, and percent used.
 */
DiskStatus Monitor::check_disk_health(const std::string& path) {
    DiskStatus status = {0, 0, 0.0};
#ifdef _WIN32
    // Windows API for large integer storage math
    ULARGE_INTEGER freeB, totalB, totalFreeB;
    if (GetDiskFreeSpaceExA(path.c_str(), &freeB, &totalB, &totalFreeB)) {
        status.free_bytes = totalFreeB.QuadPart;
        status.total_bytes = totalB.QuadPart;
    }
#else
    // Linux system call for file system statistics
    struct statvfs vfs;
    if (statvfs(path.c_str(), &vfs) == 0) {
        status.total_bytes = (unsigned long long)vfs.f_blocks * vfs.f_frsize;
        status.free_bytes = (unsigned long long)vfs.f_bfree * vfs.f_frsize;
    }
#endif
    
    // Safety check to avoid division by zero
    if (status.total_bytes > 0) {
        unsigned long long used = status.total_bytes - status.free_bytes;
        status.percent_used = (static_cast<double>(used) / status.total_bytes) * 100.0;
    }
    return status;
}

/**
 * @brief Performs a "TCP Ping" to verify if a database service is reachable.
 * * This function attempts a standard three-way TCP handshake. It does not 
 * authenticate; it only verifies if the service port is listening.
 * * @param ip: The IP address of the database server.
 * @param port: The port number (e.g., 3306 for MySQL, 5432 for PostgreSQL).
 * @return true if connection is successful, false otherwise.
 */
bool Monitor::check_database_health(const std::string& ip, int port) {
#ifdef _WIN32
    // Initialize Windows Socket API
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return false;
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
#else
    // Standard Unix socket creation
    int s = socket(AF_INET, SOCK_STREAM, 0);
#endif

    if (s < 0) return false;

    // Define the server address and port
    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port); // Host-to-network-short byte order
    server.sin_addr.s_addr = inet_addr(ip.c_str());

    // Attempt the connection handshake
    int res = connect(s, (struct sockaddr*)&server, sizeof(server));

    // Cleanup resources
#ifdef _WIN32
    closesocket(s); 
    WSACleanup();
#else
    close(s);
#endif

    return (res == 0); // Success if result is 0
}

/**
 * @brief XOR Cipher for basic data obfuscation.
 * * Uses a symmetric key to scramble/unscramble data. 
 * Running this twice with the same key restores the original string.
 */
std::string Monitor::encrypt_decrypt(const std::string& data) {
    std::string result = data;
    for(size_t i = 0; i < data.size(); ++i) {
        // Modulo ensures we loop through the key if data is longer than the key
        result[i] = data[i] ^ key[i % key.size()];
    }
    return result;
}

/**
 * @brief Logs encrypted messages to a file in a thread-safe manner.
 * * Uses std::lock_guard to prevent race conditions if multiple threads 
 * attempt to write to the same file simultaneously.
 * * @param message: The plain-text alert message.
 */
void Monitor::log_alert(const std::string& message) {
    // RAII lock: Automatically unlocks when 'lock' goes out of scope
    std::lock_guard<std::mutex> lock(mtx); 
    
    std::string encrypted = encrypt_decrypt(message);
    
    // Open in append and binary mode to prevent OS-specific line-ending corruption
    std::ofstream log_file(log_filename, std::ios::app | std::ios::binary);

    if(!log_file.is_open()) return;
    
    log_file << encrypted << "\n";
    log_file.close();
}

/**
 * @brief Sends a native system notification.
 * 
 * Windows: Uses MessageBox for notifications (runs in separate thread to avoid blocking)
 * Linux: Uses notify-send command (requires libnotify-bin package)
 * 
 * @param title: Notification title
 * @param message: Notification message
 * @param level: Severity level (affects icon/color)
 * @return true if notification sent successfully
 */
bool Monitor::send_system_notification(
    const std::string& title, 
    const std::string& message,
    NotificationLevel level
) {
#ifdef _WIN32
    // Windows implementation using MessageBox
    
    UINT icon_type;
    switch(level) {
        case NotificationLevel::INFO:
            icon_type = MB_ICONINFORMATION;
            break;
        case NotificationLevel::WARNING:
            icon_type = MB_ICONWARNING;
            break;
        case NotificationLevel::CRITICAL:
            icon_type = MB_ICONERROR;
            break;
        default:
            icon_type = MB_ICONINFORMATION;
    }
    
    // Create a separate thread so notification doesn't block monitoring
    std::thread([title, message, icon_type]() {
        MessageBoxA(
            NULL,
            message.c_str(),
            title.c_str(),
            icon_type | MB_OK | MB_TOPMOST | MB_SETFOREGROUND
        );
    }).detach();
    
    return true;
    
#else
    // Linux implementation using notify-send
    // Requires: sudo apt-get install libnotify-bin
    
    std::string urgency;
    std::string icon;
    
    switch(level) {
        case NotificationLevel::INFO:
            urgency = "low";
            icon = "dialog-information";
            break;
        case NotificationLevel::WARNING:
            urgency = "normal";
            icon = "dialog-warning";
            break;
        case NotificationLevel::CRITICAL:
            urgency = "critical";
            icon = "dialog-error";
            break;
    }
    
    // Build notify-send command
    std::string command = "notify-send ";
    command += "-u " + urgency + " ";
    command += "-i " + icon + " ";
    command += "\"" + title + "\" ";
    command += "\"" + message + "\" ";
    command += "2>/dev/null &";  // Run in background, suppress errors
    
    // Execute command
    int result = system(command.c_str());
    
    return (result == 0);
#endif
}

/**
 * @brief The main execution loop for the DeepGuard agent.
 * * Periodically evaluates system load, disk space, and database status.
 * If any metric exceeds thresholds, it triggers a secure log event and system notification.
 * * @param interval_seconds: Frequency of checks.
 */
void Monitor::run_monitoring_cycle(int interval_seconds) {
    while(true) {
        // --- 1. SENSOR READINGS ---
        
        float current_load = read_system_load();
        
        #ifdef _WIN32
            DiskStatus ds = check_disk_health("C:\\");
        #else
            DiskStatus ds = check_disk_health("/");
        #endif

        // Example: Check for a local MySQL instance
        bool db_up = check_database_health("127.0.0.1", 3306);

        // --- 2. EVALUATION & ALERTING ---
        
        // Trigger alert if Load is high, DB is down, or Disk is > 90% full
        if(current_load > load_threshold || !db_up || ds.percent_used > 90.0) {
            std::string alert = "CRITICAL: Load=" + std::to_string(current_load) + 
                                " | Disk=" + std::to_string(ds.percent_used) + "%" +
                                " | DB=" + (db_up ? "UP" : "DOWN");
            
            // Log the alert (encrypted)
            log_alert(alert);
            std::cout << "[Monitor] Alert triggered and logged.\n";
            
            // Determine notification severity
            NotificationLevel level = NotificationLevel::WARNING;
            if(current_load > load_threshold * 1.5 || ds.percent_used > 95.0) {
                level = NotificationLevel::CRITICAL;
            }
            
            // Send system notification
            send_system_notification("DeepGuard Alert", alert, level);
            
        } else {
            // Heartbeat output for console monitoring
            std::cout << "[Monitor] System OK. Load: " << current_load 
                      << " | Disk: " << ds.percent_used << "% | DB: UP\n";
        }

        // --- 3. COOL DOWN ---
        std::this_thread::sleep_for(std::chrono::seconds(interval_seconds));
    }
}