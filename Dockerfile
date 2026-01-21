# Stage 1: Build
FROM alpine:latest AS builder
WORKDIR /app
COPY . .

# Install build dependencies (g++, make, etc.)
RUN apk add --no-cache build-base

# Compile (added -pthread for thread support)
RUN g++ -I./include -Wall -std=c++17 -pthread \
    src/Monitor.cpp src/Config.cpp src/main.cpp \
    -o health_monitor

# Stage 2: Runtime
FROM alpine:latest
WORKDIR /root/

# Install runtime dependencies (libstdc++ and libgcc for thread support)
RUN apk add --no-cache libstdc++ libgcc

# Copy the compiled binary
COPY --from=builder /app/health_monitor .

# Make executable (just in case)
RUN chmod +x health_monitor

# The container is "Key-Agnostic" until you launch it.
CMD ["./health_monitor"]