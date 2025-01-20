// LINUX FILE MANAGER
// -------------------
// author:  @techapostle (Kian Dadkhah)
// date:    1/18/2025
// license: GPL-3.0
// version: 0.1.0 (alpha)
// -------------------
// description:
// This is a simple file manager for Linux systems. It is written in C++ and uses the ncurses library for the user interface.
// -------------------
// usage:
// $ ./lfm
// -------------------
// dependencies:
// - ncurses
// -------------------

#include <iostream> // for error reporting

#include "core/FileManager.h" // include the FileManager class

int main(int argc, char *argv[]) {
  using namespace linux_file_manager::core;

  // Check if the user provided a path as a command-line argument
  if (argc < 2) {
    // If no path was provided, print an error message and exit
    std::cerr << "Usage: " << argv[0] << " <path>" << std::endl;
    return 1;
  }

  // Get the path from the command-line arguments
  const std::string path = argv[1];

  // Check if the path exists
  if (!FileManager::exists(path)) {
    // If the path does not exist, print an error message and exit
    std::cerr << "Error: Path does not exist: " << path << std::endl;
    return 1;
  }

  return 0;
}