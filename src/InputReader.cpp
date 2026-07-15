#include "InputReader.h"

#include <iostream>
#include <print>
#include <stdexcept>
#include <string>

std::optional<int> readIntFromUser() {
	std::print("Enter a number: ");
	std::string input;
	std::cin >> input;

	try {
		int val = std::stoi(input);
		return val;
	} catch (const std::invalid_argument &e) {
		std::println("Invalid input: {}", e.what());
		return std::nullopt;
	}
}

std::string readString(const std::string &paramName) {
	std::string input;

	std::print("Enter {}: ", paramName);
	std::getline(std::cin, input);

	return input;
}
