#include "InputReader.h"
#include "Student.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <json/json.h>
#include <string>
#include <vector>

#define FILENAME "students.json"

/**
 * @brief Deprecated: Dynamic generation of students from input
 */
void studentInput(Student &student) {
	std::string name;

	do {
		name = readString("Name");
	} while (name.empty());

	student.setName(std::move(name));

	std::cout << "Enter age (10-100):" << std::endl;
	for (;;) {
		if (const auto result = readIntFromUser(); result.has_value()) {
			try {
				student.setAge(result.value());
				break;
			} catch (const std::invalid_argument &e) {
				std::cout << "Error: " << e.what() << ". Please enter age between 10 and 100." << std::endl;
			}
		} else {
			std::cout << "Failed to read number. Try again." << std::endl;
		}
	}
}

/**
 * @brief Load students from a JSON file
 *
 * @param filename path to JSON file
 * @return vector of parsed students
 *
 * TODO: потом попробовать оптимизировать через потоки
 */
std::vector<Student> loadStudentsFromJson(const std::string &filename) {
	std::vector<Student> students;

	Json::Value json;
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cout << "Could not open " << filename << std::endl;
		return students;
	}

	file >> json;
	std::cout << "\nJSON is Loaded. Length: " << GREEN << json.size() << RESET << "\n" << std::endl;
	file.close();

	for (const auto &item : json) {
		Student s;

		try {
			s.setName(item["name"].asString());
			s.setAge(item["age"].asInt());
			s.setActivity(static_cast<Activity>(item["activity"].asInt()));

			students.push_back(s);
		} catch (const std::exception &e) {
			continue;
		}
	}

	return students;
}

/**
 * @brief Print all students to stdout
 *
 * @param students vector of students to print
 */
void printStudents(const std::vector<Student> &students) {
	for (int i = 0; i < students.size(); ++i) {
		if (i % 100 == 0) {
			students[i].print(i);
		}
	}
}

void printSummary(const std::vector<Student> &students, const auto &start, const auto &end) {
	std::cout << "\n=== [INFO] (" << std::fixed << std::setprecision(4)
	          << std::chrono::duration<double>(end - start).count() << "s) ===" << std::endl;
	std::cout << "\n- Total students: " << students.size() << std::endl;
	std::cout << "- Study: " << std::count_if(students.begin(), students.end(), [](const Student &s) {
		return s.getActivity() == Activity::STUDY;
	}) << std::endl;
	std::cout << "- Exercise: " << std::count_if(students.begin(), students.end(), [](const Student &s) {
		return s.getActivity() == Activity::EXERCISE;
	}) << "\n" << std::endl;
}

int main() {
	auto start = std::chrono::system_clock::now();

	auto students = loadStudentsFromJson(FILENAME);
	printStudents(students);

	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed = end - start;

	printSummary(students, start, end);

	return 0;
}
