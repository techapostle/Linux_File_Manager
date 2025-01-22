#include <iostream>
#include <ncurses.h>
#include <filesystem>

#include "../core/FileManager.h"
#include "TUI.h"

namespace linux_file_manager {
namespace tui {

using namespace linux_file_manager::core;

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

void TUI::run() {
  // Get the current directory
  std::string currentPath = ".";

  while (true) {
    // Refresh the directory contents
    this->directoryContents = FileManager::listDirectory(currentPath);

    // Display the directory
    displayDirectory(currentPath);

    // Handle user input
    int key = getch(); // Get a character from the user

    if (key == 'q') {
      break; // Exit the loop if the user presses 'q'
    } else if (key == KEY_UP) {
      // Move the selection up
      selectedIndex = (selectedIndex + 1) % directoryContents.size();
    } else if (key == KEY_DOWN) {
      // Move the selection down
      selectedIndex = (selectedIndex - 1 + directoryContents.size()) % directoryContents.size();
    } else if (key == '\n') {
      // Enter to navigate into a directory or display file information
      if (!directoryContents.empty()) {
        std::string selectedPath = directoryContents[selectedIndex];
        if (FileManager::exists(selectedPath) && std::filesystem::is_directory(selectedPath)) { // Check if path is a directory
          currentPath = selectedPath; // Update the current path
          selectedIndex = 0; // Reset the selection index
        } else {
          // Display file information
          mvprintw(directoryContents.size() + 2, 0, "File: %s", selectedPath.c_str());
          mvprintw(directoryContents.size() + 3, 0, "Size: %lu bytes", FileManager::size(selectedPath));
          getch();
        }
      }
    }
  }
}

void TUI::displayDirectory(const std::string& path) {
  clear(); // Clear the screen
  mvprintw(0, 0, "Current Directory: %s (Press 'q' to quit)", path.c_str()); // Print the current directory path
  mvprintw(1, 0, "-----------------------------------------");

  // Print the directory contents
  for (size_t i = 0; i < directoryContents.size(); ++i) {
    if (i == selectedIndex) {
      attron(A_REVERSE); // Highlight the selected item
      mvprintw(i + 2, 0, "%s", directoryContents[i].c_str()); // Print the selected item
      attroff(A_REVERSE); // Turn off highlighting
    } else {
      mvprintw(i + 2, 0, "%s", directoryContents[i].c_str()); // Print the item
    }
  }

  refresh(); // Refresh the screen
}

// void TUI::handleUserInput(const std::string& path) {
  
// }

} // namespace tui
} // namespace linux_file_manager