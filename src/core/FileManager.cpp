#include <iostream> // for error reporting
#include <filesystem> // for file system operations
#include <cstdint> // for std::uintmax_t
#include <unordered_map> // for std::unordered_map

#include "FileManager.h" // include the FileManager class

namespace fs = std::filesystem;

namespace linux_file_manager {
namespace core {

// Cache entry structure
struct CacheEntry {
  std::uintmax_t size;                     // Cached size
  std::filesystem::file_time_type time;    // Last modification time
};

// Global cache for directory sizes
std::unordered_map<std::string, CacheEntry> sizeCache;

std::vector<std::string> FileManager::listDirectory(const std::string& path) {
  // Create a vector to store the names of the files and directories within the directory specified by the path
  std::vector<std::string> contents;

  // Try to iterate over the contents of the directory
  try {
    // Iterate over the contents of the directory specified by the path
    for (const auto& entry : fs::directory_iterator(path)) {
      // Add the full path of the file or directory to the contents vector
      contents.push_back(fs::canonical(entry.path().string()));
    }
  } catch (const fs::filesystem_error& e) {
    // If an error occurs, print an error message
    std::cerr << "\nError listing directory contents: " << e.what() << std::endl;
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
    std::cerr << "\nError creating directory: " << e.what() << std::endl;
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
    std::cerr << "\nError deleting file or directory: " << e.what() << std::endl;
  }

  return false; // return false if an error occurred
}

std::uintmax_t FileManager::size(const std::string& path) {
  // Try to get the size of the file or directory
  try {
    // Skip problematic paths (e.g., symbolic links or special files, like /dev/null, /proc, /sys, etc.)
    if (fs::is_symlink(path) || fs::is_other(path)) {
      return 0; // Return 0 for symbolic links and special files
    }
    if (path == "/proc" || path == "/sys") { // May be redundant, but just in case
      return 0; // Return 0 for /proc and /sys
    }

    // Check if the path is a directory
    if (fs::is_directory(path)) {
      auto lastWriteTime = fs::last_write_time(path); // Get the last write time of the directory

      // Check if the directory size is already cached
      auto cacheIt = sizeCache.find(path); // Search for the path in the cache
      if (cacheIt != sizeCache.end() && cacheIt->second.time == lastWriteTime) {
        return cacheIt->second.size; // Return cached size if unchanged
      }

      // Calculate the size of the directory otherwise
      std::uintmax_t total_size = 0;
      for (const auto& entry : fs::recursive_directory_iterator(path)) {
        if (fs::is_regular_file(entry.path())) {
          total_size += fs::file_size(entry.path());
        }
      }

      // Update the cache with the new size
      sizeCache[path] = {total_size, lastWriteTime};

      return total_size; // Return the total size of the directory
    }

  } catch (const fs::filesystem_error& e) {
    // If an error occurs, print an error message and return 0
    std::cerr << "\nError getting file or directory size: " << e.what() << std::endl;
  }

  return 0; // return 0 by default or if an error occurred
}

} // namespace core
} // namespace linux_file_manager