#pragma once

#include "Activity.h"

#include <string>

/**
 * @brief Utility class for string operations
 */
class StringHelper {
public:
	/**
	 * @brief Trim whitespace characters from both ends of a string
	 *
	 * @param str string to trim
	 * @return trimmed string
	 */
	static std::string trim(const std::string &str) {
		const auto trimChars = " \t\n\r";

		const size_t start = str.find_first_not_of(trimChars);
		const size_t end = str.find_last_not_of(trimChars);

		return str.substr(start, end - start + 1);
	}

	/**
	 * @brief Convert Activity enum to string
	 *
	 * @param activity activity type
	 * @return string representation
	 */
	static std::string getEnumName(Activity activity) {
		switch (activity) {
			case STUDY:
				return "Study";
			case EXERCISE:
				return "Exercise";
			default:
				return "";
		}
	}
};
