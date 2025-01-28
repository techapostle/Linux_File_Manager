#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <cstdint>
#include <filesystem>
#include <unordered_map>

namespace linux_file_manager {
namespace utils {

/**
 * @brief A struct to represent a SizeCache entry for directory sizes
 * @details This struct stores the size of a directory and the last modification time
 * of the directory for caching purposes.
 */
struct SizeCache {
  std::uintmax_t size;                     // Cached size
  std::filesystem::file_time_type time;    // Last modification time
};

/**
 * @brief A function to load the size cache from a file
 * @param path The path to the file to load the cache from
 * @return The cache object
 */
std::unordered_map<std::string, SizeCache> loadSizeCache(const std::string& path);

/**
 * @brief A function to save the size cache to a file
 * @param path The path to the file to save the cache to
 * @param cache The cache object to save
 * @return void
 */
void saveSizeCache(const std::string& path, const std::unordered_map<std::string, SizeCache>& cache);

} // namespace utils
} // namespace linux_file_manager

#endif // UTILS_H