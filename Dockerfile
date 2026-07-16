FROM ubuntu:24.04

# Install build tools
RUN apt-get update && apt-get install -y \
    build-essential \
    clang \
    cmake \
    gdb \
    valgrind \
    ninja-build \
    git \
    curl \
    && rm -rf /var/lib/apt/lists/*

# Optional: install libraries for HTTP / async
RUN apt-get update && apt-get install -y \
    libpthread-stubs0-dev \
    libcmocka-dev \
    libpcre2-dev

# Set working directory
WORKDIR /project
ENTRYPOINT ["/bin/bash"]