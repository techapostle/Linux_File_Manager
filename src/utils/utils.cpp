#include "utils.h"
#include <fstream> // For file I/O
#include <iostream> // For error reporting

namespace linux_file_manager {
namespace utils {

std::unordered_map<std::string, SizeCache> loadSizeCache(const std::string& path) {
  std::unordered_map<std::string, SizeCache> cache;

  // Open the file in binary read mode
  std::ifstream inFile(path, std::ios::binary);
  if (!inFile) {
    std::cerr << "Cache file not found or unreadable: " << path << std::endl;
    return cache;
  }

  size_t numEntries = 0;
  inFile.read(reinterpret_cast<char*>(&numEntries), sizeof(numEntries)); // Read the number of entries

  for (size_t i = 0; i < numEntries; ++i) {
    size_t pathSize = 0;
    inFile.read(reinterpret_cast<char*>(&pathSize), sizeof(pathSize)); // Read the size of the path string

    std::string path(pathSize, '\0');
    inFile.read(&path[0], pathSize); // Read the path string

    SizeCache entry;
    inFile.read(reinterpret_cast<char*>(&entry.size), sizeof(entry.size)); // Read the cached size

    // Read the file time as a duration
    std::chrono::system_clock::duration duration;
    inFile.read(reinterpret_cast<char*>(&duration), sizeof(duration));
    entry.time = std::filesystem::file_time_type(duration);

    cache[path] = entry; // Add to the cache
  }

  inFile.close();
  return cache;
}

void saveSizeCache(const std::string& path, const std::unordered_map<std::string, SizeCache>& cache) {
  // Open the file in binary write mode
  std::ofstream outFile(path, std::ios::binary);
  if (!outFile) {
    std::cerr << "Failed to open cache file for writing: " << path << std::endl;
    return;
  }

  size_t numEntries = cache.size();
  outFile.write(reinterpret_cast<const char*>(&numEntries), sizeof(numEntries)); // Write the number of entries

  for (const auto& [key, entry] : cache) {
    size_t pathSize = key.size();
    outFile.write(reinterpret_cast<const char*>(&pathSize), sizeof(pathSize)); // Write the size of the path string
    outFile.write(key.data(), pathSize); // Write the path string

    outFile.write(reinterpret_cast<const char*>(&entry.size), sizeof(entry.size)); // Write the cached size

    // Write the file time as a duration
    auto duration = entry.time.time_since_epoch();
    outFile.write(reinterpret_cast<const char*>(&duration), sizeof(duration));
  }

  outFile.close();
}

} // namespace utils
} // namespace linux_file_manager
