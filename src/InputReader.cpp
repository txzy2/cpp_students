#include "InputReader.h"

#include <iostream>
#include <stdexcept>
#include <string>

/**
 * @brief Read an integer within a specific range
 */
int readIntInRange(const std::string& prompt, const int min_value, const int max_value) {
	std::cout << prompt << " (" << min_value << "-" << max_value << "): ";

	while (true) {
		std::string input;
		std::getline(std::cin, input);

		try {
			if (const int value = std::stoi(input); value >= min_value && value <= max_value) {
				return value;
			}
			std::cout << "Error: Please enter a number between "
					  << min_value << " and " << max_value << "." << std::endl;
		} catch (const std::exception&) {
			std::cout << "Error: Invalid number. Please try again." << std::endl;
		}
	}
}


std::string readString(const std::string &paramName) {
	std::string input;

	std::cout << "Enter " << paramName << ": ";
	std::getline(std::cin, input);

	return input;
}
