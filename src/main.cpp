#include "InputReader.h"
#include "Student.h"

#include <fstream>
#include <json/json.h>
#include <print>
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

	std::println("Enter age (10-100):");
	for (;;) {
		if (const auto result = readIntFromUser(); result.has_value()) {
			try {
				student.setAge(result.value());
				break;
			} catch (const std::invalid_argument &e) {
				std::println("Error: {}. Please enter age between 10 and 100.", e.what());
			}
		} else {
			std::println("Failed to read number. Try again.");
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
		std::println("Could not open {}", filename);
		return students;
	}

	file >> json;
	std::println("\nJSON is Loaded. Length: {}{}{}\n", GREEN, json.size(), RESET);
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
	std::println("\n=== [INFO] ({:.4f}s) ===", std::chrono::duration<double>(end - start).count());
	std::println("\n- Total students: {}", students.size());
	std::println("- Study: {}", std::count_if(students.begin(), students.end(),
	                                          [](const Student &s) { return s.getActivity() == Activity::STUDY; }));
	std::println("- Exercise: {}\n", std::count_if(students.begin(), students.end(), [](const Student &s) {
		             return s.getActivity() == Activity::EXERCISE;
	             }));
}

int main() {
	auto start = std::chrono::system_clock::now();

	auto students = loadStudentsFromJson(FILENAME);
	printStudents(students);

	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed = end - start;

	auto studyCount = std::count_if(students.begin(), students.end(),
	                                [](const Student &s) { return s.getActivity() == Activity::STUDY; });

	printSummary(students, start, end);

	return 0;
}
