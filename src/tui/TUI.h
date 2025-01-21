#ifndef TUI_H
#define TUI_H

#include <string>
#include <cstdint>

namespace linux_file_manager {
namespace tui {

/**
 * @brief A class for managing the text-based user interface
 * @details This class provides functions for displaying the text-based user interface and interacting with the user.
 */
class TUI {
public:
  /**
   * @brief Run the text-based user interface
   * @return void
   */
  void run();

private:
  // Helper functions for navigation and actions

  /**
   * @brief Display the directory
   * @param path The path to the directory
   * @return void
   */
  void displayDirectory(const std::string& path);

  /**
   * @brief Handle user input
   * @param path The path to the current directory
   * @return void
   */
  void handleUserInput(const std::string& path);
};

} // namespace tui
} // namespace linux_file_manager

#endif // TUI_H