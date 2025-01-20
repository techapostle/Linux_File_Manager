#include <iostream> // for error reporting
#include <filesystem> // for file system operations
#include <cstdint> // for std::uintmax_t

#include "FileManager.h" // include the FileManager class

namespace fs = std::filesystem;

namespace linux_file_manager {
namespace core {

std::vector<std::string> FileManager::listDirectory(const std::string& path) {
  // Create a vector to store the names of the files and directories within the directory specified by the path
  std::vector<std::string> contents;

  // Try to iterate over the contents of the directory
  try {
    // Iterate over the contents of the directory specified by the path
    for (const auto& entry : fs::directory_iterator(path)) {
      // Add the name of the file or directory to the vector
      contents.push_back(entry.path().filename().string());
    }
  } catch (const fs::filesystem_error& e) {
    // If an error occurs, print an error message and return an empty vector
    std::cerr << "Error listing directory: " << e.what() << std::endl;
  }

  return contents; // return the vector of contents
}

bool FileManager::exists(const std::string& path) {
  // Check if the file or directory exists
  return fs::exists(path);
}

bool FileManager::createDirectory(const std::string& path) {
  // Check if the directory exists
  if (exists(path)) {
    // If the directory already exists, return false
    return false;
  }

  // Try to create the directory
  try {
    // Create the directory
    return fs::create_directory(path); // create_directory returns true if the directory was created successfully
  } catch (const fs::filesystem_error& e) {
    // If an error occurs, print an error message and return false
    std::cerr << "Error creating directory: " << e.what() << std::endl;
  }

  return false; // return false if an error occurred
}

bool FileManager::deletePath(const std::string& path) {
  // Check if the file or directory exists
  if (!exists(path)) {
    // If the file or directory does not exist, return false
    return false;
  }

  // Try to remove the file or directory
  try {
    // Remove the file or directory
    return fs::remove_all(path) > 0; // remove_all returns the number of files or directories removed
  } catch (const fs::filesystem_error& e) {
    // If an error occurs, print an error message and return false
    std::cerr << "Error deleting file or directory: " << e.what() << std::endl;
  }

  return false; // return false if an error occurred
}

std::uintmax_t FileManager::size(const std::string& path) {
  // Try to get the size of the file or directory
  try {
    if (fs::is_directory(path)) {
      std::uintmax_t total_size = 0;
      for (const auto& entry : fs::recursive_directory_iterator(path)) {
        if (fs::is_regular_file(entry.path())) {
          total_size += fs::file_size(entry.path());
        }
      }
      return total_size;
    } else {
      return fs::file_size(path); // file_size returns the size of the file in bytes
    }
  } catch (const fs::filesystem_error& e) {
    // If an error occurs, print an error message and return 0
    std::cerr << "Error getting file or directory size: " << e.what() << std::endl;
  }

  return 0; // return 0 if an error occurred
}

} // namespace core
} // namespace linux_file_manager