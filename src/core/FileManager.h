#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>
#include <vector>
#include <cstdint>

namespace linux_file_manager {
namespace core {


class FileManager {
public:
  /**
  * @brief List the contents of a directory
  * @param path The path to the directory
  * @return A vector of strings containing the names of the files and directories in the directory
  */
  static std::vector<std::string> listDirectory(const std::string &path);

  /**
  * @brief Check if a file or directory exists
  * @param path The path to the file or directory
  * @return True if the file or directory exists, false otherwise
  */
  static bool exists(const std::string& path);

  /**
  * @brief Create a new directory
  * @param path The path to the new directory
  * @return True if the directory was created successfully, false otherwise
  */
  static bool createDirectory(const std::string& path);

  /**
  * @brief Remove a file or directory
  * @param path The path to the file or directory to remove
  * @return True if the file or directory was removed successfully, false otherwise
  */
  static bool deletePath(const std::string& path);

  /**
  * @brief Get file or directory size
  * @param path The path to the file or directory
  * @return The size of the file or directory in bytes
  */
  static std::uintmax_t size(const std::string& path);
  
};

} // namespace core
} // namespace linux_file_manager

#endif // FILE_MANAGER_H