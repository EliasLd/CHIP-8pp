# Build stage
FROM debian:bookworm-slim AS builder

RUN apt-get update && apt-get install -y \
    cmake \
    make \
    g++ \
    git \
    libsdl2-dev \
    libpulse0 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

RUN mkdir -p build && cd build && cmake .. && make

# Final small image
FROM debian:bookworm-slim

RUN apt-get update && apt-get install -y \
    libsdl2-2.0-0 \
    libx11-6 \
    libxext6 \
    libxrandr2 \
    libxrender1 \
    libxfixes3 \
    libxcursor1 \
    libxi6 \
    libgl1 \
    libpulse0 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=builder /app/build ./build

ENTRYPOINT [ "./build/emulator" ]