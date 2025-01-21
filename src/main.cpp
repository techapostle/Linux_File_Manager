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

// #define __TEST__ // uncomment this line to enable testing
#ifdef __BASIC_LFM_TEST__

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

  // Testing all current functions
  while (true) {
    std::cout << "\nThe path you entered is: " << path << std::endl;
    std::cout << "What would you like to do?" << std::endl;

    std::cout << "0. Exit" << std::endl;
    std::cout << "1. Check if the file or directory exists" << std::endl;
    std::cout << "2. List the contents of the directory" << std::endl;
    std::cout << "3. Create a new directory" << std::endl;
    std::cout << "4. Remove a file or directory" << std::endl;
    std::cout << "5. Get file or directory size" << std::endl;

    char choice;
    std::cout << "Enter your choice: ";
    std::cin >> choice;

    switch (choice) {
      case '0':
        return 0;
      case '1':
        std::cout << "Checking if the file or directory exists..." << std::endl;
        if (FileManager::exists(path)) {
          std::cout << "The file or directory exists." << std::endl;
        } else {
          std::cout << "The file or directory does not exist." << std::endl;
        }
        break;
      case '2':
        std::cout << "Listing the contents of the directory..." << std::endl;
        for (const auto& entry : FileManager::listDirectory(path)) {
          std::cout << entry << std::endl;
        }
        break;
      case '3':
        std::cout << "Creating a new directory..." << std::endl;
        if (FileManager::createDirectory(path)) {
          std::cout << "The directory was created successfully." << std::endl;
        } else {
          std::cout << "An error occurred while creating the directory." << std::endl;
        }
        break;
      case '4':
        std::cout << "Removing a file or directory..." << std::endl;
        if (FileManager::deletePath(path)) {
          std::cout << "The file or directory was removed successfully." << std::endl;
        } else {
          std::cout << "An error occurred while removing the file or directory." << std::endl;
        }
        break;
      case '5':
        std::cout << "Getting file or directory size..." << std::endl;
        std::cout << "The size of the file or directory is: " << FileManager::size(path) << " bytes." << std::endl;
        break;
      default:
        std::cout << "Invalid choice. Please try again." << std::endl;
        break;
    }
  }

  return 0;
}

#endif