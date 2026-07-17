#pragma once
#include <iostream>

/**
 * @brief Display the main menu options to stdout
 *
 * Prints available commands: Add, Find, Delete, Edit student, or Exit.
 */
void getMenu() {
	std::cout << "==== MENU ====" << "\n"
	          << "1. Add Student" << "\n"
	          << "2. Find Student (By ID)" << "\n"
	          << "3. Del Student (By ID)" << "\n"
	          << "4. Edit Student" << "\n"
	          << "0. Exit" << "\n"
	          << "============\n";
}

/**
 * @brief Display the edit submenu options to stdout
 *
 * Prints available edit commands for a selected student.
 */
void editMenu() {
	std::cout << "==== EDIT MENU ====" << "\n"
	          << "1. Edit Name" << "\n";
}
