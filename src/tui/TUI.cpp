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
  initscr();           // Initialize the ncurses screen
  cbreak();            // Disable line buffering
  noecho();            // Do not echo user input
  keypad(stdscr, TRUE); // Enable keypad input
  curs_set(0);         // Hide the cursor

  // Enable colors
  if (has_colors()) {
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);   // Program info and current directory
    init_pair(2, COLOR_WHITE, COLOR_BLUE);   // Highlighted item
    init_pair(3, COLOR_YELLOW, COLOR_BLACK); // Normal text
    init_pair(4, COLOR_RED, COLOR_BLACK);    // Error messages
    init_pair(5, COLOR_GREEN, COLOR_BLACK);  // Legend keys
  }
}

void TUI::displayHeader(const std::string& currentPath) {
  attron(COLOR_PAIR(1)); // Use cyan color for the header
  mvprintw(0, 0, "Linux File Manager (Press 'q' to quit)");
  mvprintw(1, 0, "Current Directory: %s", currentPath.c_str());
  attroff(COLOR_PAIR(1));
}

void TUI::cleanup() {
  endwin(); // End the ncurses session
}

void TUI::run(std::string path) {
  std::string currentPath = std::filesystem::canonical(path).string();
  std::string errorMessage;

  while (true) {
    // Refresh the directory contents
    directoryContents = FileManager::listDirectory(currentPath);

    // Add the parent directory entry if not at the root
    if (currentPath != "/") {
      directoryContents.insert(directoryContents.begin(), currentPath + "/..");
    }

    // Render the TUI layout
    displayHeader(currentPath); // Display the header with program information and the current directory
    displayDirectory(currentPath); // Display the directory pane and file details
    displayFooter(errorMessage); // Display the footer with error messages and legend keys

    // Get user input and handle it
    int key = getch();
    if (key == 'q') {
      break; // Quit the program
    }

    try {
      currentPath = handleUserInput(currentPath, key);
      errorMessage.clear(); // Clear error messages after successful input handling
    } catch (const std::exception& e) {
      errorMessage = e.what(); // Capture and display any error messages
    }
  }
}

void TUI::displayDirectory(const std::string& currentPath) {
  clear(); // Clear the screen

  // Render the top header
  displayHeader(currentPath);

  // Render the left pane (directory listings)
  int leftPaneWidth = COLS / 2; // Half the screen width
  std::string displayName;
  for (size_t i = 0; i < directoryContents.size(); ++i) {
    // Extract the filename or display name from the full path
    displayName = fs::path(directoryContents[i]).filename().string();

    if (i == selectedIndex) {
      // Highlight the selected item
      attron(COLOR_PAIR(2));
      mvprintw(3 + i, 0, "%-*s", leftPaneWidth, displayName.c_str());
      attroff(COLOR_PAIR(2));
    } else {
      // Print normal text
      attron(COLOR_PAIR(3));
      mvprintw(3 + i, 0, "%-*s", leftPaneWidth, displayName.c_str());
      attroff(COLOR_PAIR(3));
    }
  }

  // Render the right pane (file details)
  if (selectedIndex < directoryContents.size()) {
    std::string selectedPath = directoryContents[selectedIndex];
    if (FileManager::exists(selectedPath)) {
      attron(COLOR_PAIR(3));
      mvprintw(3, leftPaneWidth + 2, "File Info:");
      mvprintw(4, leftPaneWidth + 2, "Path: %s", selectedPath.c_str());
      mvprintw(5, leftPaneWidth + 2, "Size: %lu bytes", FileManager::size(selectedPath));
      attroff(COLOR_PAIR(3));
    }
  }
}

void TUI::displayFooter(const std::string& errorMessage) {
  int bottomRow = LINES - 3; // Three lines from the bottom

  // Render the error message
  if (!errorMessage.empty()) {
    attron(COLOR_PAIR(4)); // Red for errors
    mvprintw(bottomRow, 0, "Error: %s", errorMessage.c_str());
    attroff(COLOR_PAIR(4));
  }

  // Render the legend
  attron(COLOR_PAIR(5)); // Green for the legend
  mvprintw(bottomRow + 1, 0, "Legend: [UP/DOWN] Navigate  [ENTER] Open  [q] Quit");
  attroff(COLOR_PAIR(5));
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
    // call function to display file preview
    // displayPreview(currentPath, directoryContents[selectedIndex]);
  }
  // } else if (key == '\n') {
  //   // Enter to navigate into a directory or display file information
  //   if (!directoryContents.empty()) {
  //     std::string selectedPath = directoryContents[selectedIndex];
  //     if (FileManager::exists(selectedPath) && std::filesystem::is_directory(selectedPath)) { // Check if path is a directory
  //       return selectedPath; // Return the selected directory path
  //     } else {
  //       // Display file information
  //       mvprintw(directoryContents.size() + 2, 0, "File: %s", selectedPath.c_str());
  //       mvprintw(directoryContents.size() + 3, 0, "Size: %lu bytes", FileManager::size(selectedPath));
  //       getch();
  //     }
  //   }
  // }

  return currentPath; // Return the current path if no navigation occurred
}

} // namespace tui
} // namespace linux_file_manager