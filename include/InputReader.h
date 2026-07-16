#pragma once

#include <optional>
#include <string>

const std::string RESET = "\033[0m";
const std::string WHITE = "\033[37m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string RED = "\033[31m";

/**
 * @brief Read an integer from user input
 *
 * @return integer value or std::nullopt on invalid input
 */
int readIntInRange(const std::string& prompt, const int min_value, const int max_value);

/**
 * @brief Read a string from user input with a prompt
 *
 * @param paramName name to display in the prompt (e.g. "Name")
 * @return user input string
 */
std::string readString(const std::string &paramName);
