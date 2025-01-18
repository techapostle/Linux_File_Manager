cmake_minimum_required(VERSION 3.10)

# Project name
project(Linux_File_Manager VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(${CURSES_INCLUDE_PATH})

# Source files
file(GLOB_RECURSE CORE_SRC "src/core/*.cpp")
file(GLOB_RECURSE TUI_SRC "src/tui/*.cpp")

# Add executable target
add_executable(Linux_File_Manager src/main.cpp ${CORE_SRC} ${TUI_SRC})

# Link ncurses library
find_package(Curses REQUIRED)
target_link_libraries(Linux_File_Manager ${CURSES_LIBRARIES})

