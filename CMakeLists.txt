cmake_minimum_required(VERSION 3.1)
project(chip-8pp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/external)

# First fetch the SDL2 library
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.30.1
)

FetchContent_MakeAvailable(SDL2)

include_directories(${CMAKE_SOURCE_DIR}/include)

# Get all .cpp source files
file(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(emulator ${SRC_FILES})

target_link_libraries(emulator PRIVATE SDL2)
target_include_directories(emulator PRIVATE ${sdl2_SOURCE_DIR}/include)
