#include <iostream>
#include <ncurses.h>
#include <filesystem>

#include "../core/FileManager.h"
#include "TUI.h"

namespace linux_file_manager {
namespace tui {

using namespace linux_file_manager::core;

namespace fs = std::filesystem;

TUI::TUI() : selectedIndex(0) {
  initialize();
}

TUI::~TUI() {
  cleanup();
}

void TUI::initialize() {
  initscr(); // Initialize the ncurses screen
  cbreak(); // Disable line buffering
  noecho(); // Do not echo user input
  keypad(stdscr, TRUE); // Enable keypad input
  curs_set(0); // Hide the cursor
  // refresh(); // Refresh the screen
}

void TUI::cleanup() {
  endwin(); // End the ncurses session
}

void TUI::run(std::string path) {
  // Resolve the initial path to an absolute path
  std::string currentPath = fs::canonical(path).string();

  while (true) {
    // Refresh the directory contents
    this->directoryContents = FileManager::listDirectory(currentPath);

    // Insert the parent directory entry ("..") at the beginning of the list
    // This allows the user to navigate to the parent directory
    if (currentPath != "/") {
      directoryContents.insert(directoryContents.begin(), currentPath + "/..");
    }

    // Display the directory
    displayDirectory(currentPath);

    // Handle user input
    int key = getch(); // Get a character from the user

    if (key == 'q')
      break; // Exit the loop if the user presses 'q'
    
    // Delegate input handling to `handleUserInput`
    currentPath = handleUserInput(currentPath, key);
  }
}

void TUI::displayDirectory(const std::string& path) {
  // Clear the screen to prepare for rendering the updated directory contents
  clear();

  // Display the current directory path and a quit instruction at the top
  mvprintw(0, 0, "Current Directory: %s (Press 'q' to quit)", path.c_str());
  mvprintw(1, 0, "-----------------------------------------");

  // Iterate over the directory contents and display each entry
  for (size_t i = 0; i < directoryContents.size(); ++i) {
    // Extract the filename or display name from the full path
    std::string displayName = fs::path(directoryContents[i]).filename().string();

    if (i == selectedIndex) {
      // Highlight the currently selected item
      attron(A_REVERSE);
      mvprintw(i + 2, 0, "%s", displayName.c_str());
      attroff(A_REVERSE);
    } else {
      // Print non-selected items without highlighting
      mvprintw(i + 2, 0, "%s", displayName.c_str());
    }
  }

  // Refresh the screen to display the updated contents
  refresh();
}

std::string TUI::handleUserInput(const std::string& currentPath, int key) {
  // Handle user input
  if (key == 'q') {
    return ""; // Return an empty string to indicate that the user wants to quit
  } else if (key == KEY_UP) {
    // Move the selection up
    selectedIndex = (selectedIndex - 1 + directoryContents.size()) % directoryContents.size();
  } else if (key == KEY_DOWN) {
    // Move the selection down
    selectedIndex = (selectedIndex + 1) % directoryContents.size();
  } else if (key == '\n') {
    // Enter to navigate into a directory or display file information
    if (!directoryContents.empty()) {
      std::string selectedPath = directoryContents[selectedIndex];
      if (FileManager::exists(selectedPath) && std::filesystem::is_directory(selectedPath)) { // Check if path is a directory
        return selectedPath; // Return the selected directory path
      } else {
        // Display file information
        mvprintw(directoryContents.size() + 2, 0, "File: %s", selectedPath.c_str());
        mvprintw(directoryContents.size() + 3, 0, "Size: %lu bytes", FileManager::size(selectedPath));
        getch();
      }
    }
  }

  return currentPath; // Return the current path if no navigation occurred
}

} // namespace tui
} // namespace linux_file_manager