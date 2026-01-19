# DeepGuard: Service Health Check 🛡️

A lightweight, high-performance **C++ sidecar monitor** designed to track system performance and service health in real-time. This project demonstrates modular C++ design, cross-platform system interaction (Linux & Windows), and secure encrypted logging.

## ✨ Features
* **Real-time Monitoring:** Tracks CPU load averages across different operating systems.
* **Encrypted Logging:** All health alerts are **XOR-encrypted** using a secure key from your environment before being written to disk to ensure data privacy.
* **Dockerized:** Fully containerized for easy deployment as a sidecar service.
* **Secure Configuration:** Enforces security best practices with **mandatory environment variables** for secrets. No hardcoded passwords.

---

## 🛠️ Tech Stack
* **Language:** C++17
* **Compiler:** G++ (GCC) / Clang / MSVC
* **Containerization:** Docker (Debian-slim)
* **Encryption:** Custom XOR-bitstream cipher

---

## 🚀 Getting Started

### Prerequisites
* A C++17 compatible compiler.
* Linux or Windows environment.
* (Optional) [Docker Desktop](https://www.docker.com/products/docker-desktop/)

### Building Locally

Since the source files are located in the `src/` directory, use the following command to compile manually:

1. **Compile the project:**
   ```bash
   g++ -std=c++17 -I./include src/main.cpp src/Monitor.cpp src/Config.cpp -o deepguard -lpthread

Run the application: You must export the security key to your environment before running.

Linux/macOS:
export MONITOR_KEY="my_secret_key"
./deepguard

Windows (PowerShell):
$env:MONITOR_KEY="my_secret_key"
.\deepguard.exe

Running with Docker
docker build -t health-monitor .

Run the container (Interactive): Note: We use -it to allow the program to accept your keyboard inputs.
docker run -it --rm -e MONITOR_KEY="docker_secret_key" health-monitor


📖 Usage
When the application starts, it acts as an interactive CLI to configure the monitoring session.
Input,Description,Example
Load Threshold,"CPU load limit (e.g., 0.75 for 75%)",0.75
Log Filename,File to store encrypted alerts,alerts.log
Check Interval,Frequency of health checks (in seconds),5


Example:
-------------------------------------------
      DEEP GUARD: SYSTEM SETUP         
-------------------------------------------
[1/3] Enter CPU Load Threshold (e.g. 0.75): 0.8
[2/3] Enter name for the log file: system.log
[3/3] Enter check interval in seconds: 2

========================================
  Configuration Saved. Monitoring Started.
  Target: system.log | Alert at: >0.8
  Interval: 2s | Security: ENABLED
========================================

📂 Project Structure
Saas Health check/
├── src/
│   ├── main.cpp           # CLI entry point & interactive setup
│   ├── Monitor.cpp        # Monitoring logic & encryption implementation
│   └── Config.cpp         # Secure environment-based configuration
├── include/
│   ├── Monitor.h          # Monitor class definitions
│   └── Config.h           # Config class definitions
├── .gitignore             # Prevents binaries and logs from being tracked
├── Dockerfile             # Multi-stage Docker build
└── README.md              # Project documentation
