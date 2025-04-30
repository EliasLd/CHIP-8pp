cmake_minimum_required(VERSION 3.13)
cmake_policy(SET CMP0077 NEW)

project(chip-8pp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/external)

set(SDL_AUDIO ON CACHE BOOL "" FORCE)
set(SDL_ALSA ON CACHE BOOL "" FORCE)
set(SDL_PULSEAUDIO ON CACHE BOOL "" FORCE)
set(SDL_PIPEWIRE ON CACHE BOOL "" FORCE)

FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.30.1
)

FetchContent_MakeAvailable(SDL2)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(emulator ${SRC_FILES})

target_link_libraries(emulator PRIVATE SDL2)
target_include_directories(emulator PRIVATE ${sdl2_SOURCE_DIR}/include)
