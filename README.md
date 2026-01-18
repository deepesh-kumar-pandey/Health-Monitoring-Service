# Service-Health-Check 🛡️

A lightweight, C++ based sidecar monitor designed to track system performance and service health. This project demonstrates modular C++ design, Linux system interaction, and secure logging via XOR encryption.

## Features
* **Real-time Monitoring**: Tracks CPU load averages via the Linux `/proc` filesystem.
* **Encrypted Logging**: All health alerts are XOR-encrypted before being written to disk to ensure data privacy.
* **Dockerized**: Fully containerized for easy deployment as a sidecar service.
* **Environment Configuration**: Sensitive keys and alert thresholds are managed via environment variables.

## Tech Stack
* **Language**: C++11/17
* **Containerization**: Docker (Alpine-based for small footprint)
* **Security**: Environment-based secrets and XOR-bitstream encryption.
