#include "InputReader.h"
#include "Student.h"

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <json/json.h>
#include <string>
#include <vector>

#define FILENAME "students.json"

/**
 * @brief
 */
void studentInput(Student &student) {
	std::string name;

	do {
		name = readString("Name");
	} while (name.empty());
	student.setName(std::move(name));

	student.setAge(readIntInRange("Enter age", 10, 100));
}

/**
 * @brief Load students from a JSON file
 *
 * @param filename path to JSON file
 * @return vector of parsed students
 *
 * TODO: потом попробовать оптимизировать через потоки (Если будет 100МБ+)
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

	// NOTE: Оптимизация чтобы при добавлении студента в другом месте не выделялось + 1000 сверху
	students.reserve(json.size() + 10);

	for (const auto &item : json) {
		try {
			Student s;

			s.setName(item["name"].asString());
			s.setAge(item["age"].asInt());
			s.setActivity(static_cast<Activity>(item["activity"].asInt()));

			students.push_back(std::move(s));
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

void printSummary(const std::vector<Student> &students,
                  const std::optional<std::chrono::duration<double>> elapsed = std::nullopt) {

	if (elapsed.has_value()) {
		std::cout << "\n=== [INFO] (Parse time: " << std::fixed << std::setprecision(4)
				  << elapsed.value().count() << "s) ===" << std::endl;
	} else {
		std::cout << "\n=== [INFO] ===" << std::endl;
	}

	std::cout << "\n- Total students: " << students.size() << std::endl;
	std::cout << "- Study: " << std::count_if(students.begin(), students.end(), [](const Student &s) {
		return s.getActivity() == STUDY;
	}) << std::endl;
	std::cout << "- Exercise: " << std::count_if(students.begin(), students.end(), [](const Student &s) {
		return s.getActivity() == EXERCISE;
	}) << "\n" << std::endl;
}


void getMenu() {
	std::cout << "==== MENU ====" << "\n" << "1. Add Student" << "\n" << "0. Exit" << "\n" << "============\n";
}

int main() {
	const auto start = std::chrono::system_clock::now();
	auto students = loadStudentsFromJson(FILENAME);
	printStudents(students);
	const auto end = std::chrono::system_clock::now();

	printSummary(students, end - start);

	for (;;) {
		getMenu();

		switch (readIntInRange("Choice", 0, 3)) {
			case 0:
				return 0;
			case 1: {
				Student newStudent;
				studentInput(newStudent);
				newStudent.print(students.size() + 1);
				students.push_back(newStudent);

				printSummary(students);
				break;
			}
			default:
				std::cout << "\nInvalid choice. Please try again." << std::endl;
				break;
		}
	}
}
