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
* **Containerization:** Docker (Alpine-linux)
* **Encryption:** Custom XOR-bitstream cipher

---

## 🚀 Getting Started

### Prerequisites
* A C++17 compatible compiler.
* Linux or Windows environment.
* (Optional) [Docker Desktop](https://www.docker.com/products/docker-desktop/)


