#ifndef TUI_H
#define TUI_H

#include <vector>
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
   * @brief Construct a new TUI object
   */
  TUI();

  /**
   * @brief Destroy the TUI object
   */
  ~TUI();

  /**
   * @brief Run the text-based user interface
   * @param path The path to the directory to display, defaults to the current directory
   * @return void
   */
  void run(std::string path);

private:
  // Helper functions

  /**
   * @brief Initialize the text-based user interface
   * @return void
   */
  void initialize();

  /**
   * @brief Display the header with program information and the current directory
   * @param currentPath The current directory path
   * @return void
   */
  void displayHeader(const std::string& currentPath);

  /**
   * @brief Display the directory
   * @param path The path to the directory
   * @return void
   */
  void displayDirectory(const std::string& path);

  /**
   * @brief Display the footer with error messages and legend keys
   * @param errorMessage The error message to display
   * @return void
   */
  void displayFooter(const std::string& errorMessage);

  /**
   * @brief Cleanup the text-based user interface
   * @return void
   */
  void cleanup();

  /**
   * @brief Handle user input
   * @param currentPath The current directory path
   * @param key The key pressed by the user
   * @return void
   */
  std::string handleUserInput(const std::string& currentPath, int key);

  // State variables
  std::vector<std::string> directoryContents; // The names of the files and directories in the current directory
  int selectedIndex; // The index of the selected file or directory
};

} // namespace tui
} // namespace linux_file_manager

#endif // TUI_H