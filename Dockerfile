FROM ubuntu:25.04

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

ENTRYPOINT [ "./build/emulator" ]