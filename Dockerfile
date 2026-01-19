# Stage 1: Build
FROM gcc:latest AS builder
WORKDIR /app
COPY . .
RUN g++ -I./include -Wall -std=c++17 src/Monitor.cpp src/Config.cpp main.cpp -o health_monitor -lpthread

# Stage 2: Runtime
FROM debian:bookworm-slim
WORKDIR /root/
COPY --from=builder /app/health_monitor .

# SECURITY FIX: We removed ENV MONITOR_KEY. 
# The container is now "Key-Agnostic" until you launch it.

CMD ["./health_monitor"]