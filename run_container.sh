#!/bin/bash

function help_msg() 
{
    echo "Usage: $0 [OPTIONS] ROM_PATH [SCALE] [DELAY]"
    echo
    echo "Options:"
    echo "  -h, --help        Prints this help message"
    echo "  -r, --rom         ROM path"
    echo "  -s, --scale       Resolution scale (10 by default)."
    echo "  -d, --delay       Cycle delay (1 by default)."
    echo
    echo "Example : $0 -r roms/pong.ch8 -s 15 -d 2"
}

ROM_PATH=""
SCALE=10
DELAY=1

# Arguments parsing
while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            help_msg
            exit 0
            ;;
        -r|--rom)
            ROM_PATH=$2
            shift
            ;;
        -s|--scale)
            SCALE=$2
            shift
            ;;
        -d|--delay)
            DELAY=$2
            shift
            ;;
        *)
            echo "Invalid option: $1"
            help_msg
            exit 1
            ;;
    esac
    shift
done

if [ -z "$ROM_PATH" ]; then
    echo "Error: ROM path should be specified"
    help_msg
    exit 1
fi

ROM_PATH_ABS=$(realpath "$ROM_PATH")
ROM_DIR=$(dirname "$ROM_PATH_ABS")
ROM_FILENAME=$(basename "$ROM_PATH_ABS")

# Check if xhost is activated
# Container needs it to run SDL in host
# graphical environment
if ! xhost | grep -q "local:docker"; then
    xhost +local:docker
fi

docker run --rm \
    -e DISPLAY=$DISPLAY \
    -v /tmp/.X11-unix:/tmp/.X11-unix \
    -e PULSE_SERVER=unix:${XDG_RUNTIME_DIR}/pulse/native \
    -v ${XDG_RUNTIME_DIR}/pulse/native:${XDG_RUNTIME_DIR}/pulse/native \
    -v ~/.config/pulse/cookie:/root/.config/pulse/cookie \
    --device /dev/snd \
    -v "$ROM_DIR":/roms \
    chip8pp:latest "/roms/$ROM_FILENAME" "$SCALE" "$DELAY"