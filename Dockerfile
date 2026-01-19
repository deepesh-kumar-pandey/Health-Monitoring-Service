# Stage 1: Build
FROM alpine:latest AS builder
WORKDIR /app
COPY . .
# Install build dependencies (g++, make, etc.)
RUN apk add --no-cache build-base
RUN g++ -I./include -Wall -std=c++17 src/Monitor.cpp src/Config.cpp src/main.cpp -o health_monitor

# Stage 2: Runtime
FROM alpine:latest
WORKDIR /root/
# Install runtime dependencies (libstdc++)
RUN apk add --no-cache libstdc++
COPY --from=builder /app/health_monitor .

# The container is "Key-Agnostic" until you launch it.
CMD ["./health_monitor"]