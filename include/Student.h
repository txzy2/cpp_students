#pragma once

#include "Activity.h"
#include "InputReader.h"
#include "StringHelper.h"

#include <chrono>
#include <iostream>
#include <stdexcept>
#include <string>

/**
 * @brief Represents a student with name, age and activity type
 */
class Student {
	int id = 0;
	std::string name;
	int age = 0;
	Activity activity = STUDY;

	static constexpr int MAX_AGE = 100;
	static constexpr int MIN_AGE = 10;
	static constexpr auto ERROR_AGE_MSG = "Invalid age";
	static constexpr auto ERROR_ACTIVITY_MSG = "Invalid activity";

  public:
	/**
	 * @brief Get student name
	 * @return const reference to name
	 */
	const std::string &getName() const { return name; }

	/**
	 * @brief Get student age
	 * @return age value
	 */
	int getAge() const { return age; }

	/**
	 * @brief Get student activity
	 * @return activity type
	 */
	Activity getActivity() const { return activity; }

	int getId() const { return id; }

	/**
	 * @brief Set student name (trims whitespace)
	 * @param name student name
	 */
	void setName(std::string name) { this->name = std::move(StringHelper::trim(name)); }

	/**
	 * @brief Set student age with validation
	 * @param age student age (must be between 10 and 100)
	 * @throws std::invalid_argument if age is out of range
	 */
	void setAge(const int age) {
		if (age < MIN_AGE || age > MAX_AGE) {
			throw std::invalid_argument(ERROR_AGE_MSG);
		}
		this->age = age;
	}

	/**
	 * @brief Set student activity
	 * @param activity activity type
	 */
	void setActivity(Activity activity) {
		if (activity != STUDY && activity != EXERCISE) {
			throw std::invalid_argument(ERROR_ACTIVITY_MSG);
		}
		this->activity = activity;
	}

	/**
	 * @brief Set Student id
	 * @param id
	 */
	void setId(const int id) { this->id = id; }

	/**
	 * @brief Print student info to stdout
	 */
	void print() const {
		auto now = std::chrono::system_clock::now();

		const auto activityColor = activity == STUDY ? GREEN : RED;

		std::cout << GREEN << "[INFO]" << RESET << " " << std::format("{:%Y-%m-%d %X}", now)
		          << " ID: " << id << " Name: " << name
		          << " -> Activity: " << activityColor
		          << StringHelper::getEnumName(activity) << RESET << std::endl;
	}
};
