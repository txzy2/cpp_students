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
 * @brief Interactively read student data from user input
 *
 * Prompts the user for a name, age and assigns an ID to the student.
 * Name must be non-empty, age must be between 10 and 100.
 *
 * @param student reference to student object to populate
 * @param lastId  ID to assign to the new student
 */
void studentInput(Student &student, const int lastId) {
	std::string name;

	do {
		name = readString("Name");
	} while (name.empty());
	student.setName(std::move(name));
	student.setId(lastId);

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
			s.setId(item["id"].asInt());

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
		if (i % 100 == 0 || i == students.size() - 1) {
			students[i].print();
		}
	}

	std::cout << "\n" << "=== END ===" << "\n";
}

/**
 * @brief Print summary statistics about students to stdout
 *
 * Displays total student count and a breakdown by activity type (STUDY / EXERCISE).
 * Optionally shows the elapsed parse time if provided.
 *
 * @param students vector of students to summarize
 * @param elapsed  optional duration of JSON parsing, displayed in seconds
 */
void printSummary(const std::vector<Student> &students,
                  const std::optional<std::chrono::duration<double>> elapsed = std::nullopt) {

	if (elapsed.has_value()) {
		std::cout << "\n=== [INFO] (Parse time: " << std::fixed << std::setprecision(4) << elapsed.value().count()
		          << "s) ===" << std::endl;
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

/**
 * @brief Search for a student by ID in the given list
 *
 * Prompts the user to enter a student ID within the valid range.
 * Returns a pointer to the found student, or nullptr if not found or list is empty.
 *
 * @param students vector of students to search
 * @return const pointer to the found Student, or nullptr if not found
 */
const Student *findStudent(const std::vector<Student> &students) {
	if (students.empty()) {
		std::cout << "No students available." << std::endl;
		return nullptr;
	}

	const int maxId = std::max_element(students.begin(), students.end(), [](const Student &a, const Student &b) {
		                  return a.getId() < b.getId();
	                  })->getId();

	int id = readIntInRange("Enter student ID", 1, maxId);

	for (auto &s : students) {
		if (s.getId() == id)
			return &s;
	}

	std::cout << "\nStudent with ID " << id << " not found.\n" << std::endl;
	return nullptr;
}

/**
 * @brief Main interaction loop for managing students
 *
 * Displays the menu and handles user choices: add, find, delete, or edit students.
 * Loops until the user selects Exit (0).
 *
 * @param students reference to the vector of students to manage
 * @return 0 when the user chooses to exit
 */
int workWithStudent(std::vector<Student> &students) {
	for (;;) {
		getMenu();

		switch (readIntInRange("Choice", 0, 4)) {
		case 0:
			return 0;
		case 1: {
			Student newStudent;
			studentInput(newStudent, students.back().getId() + 1);

			newStudent.print();
			students.push_back(newStudent);

			printSummary(students);
			break;
		}
		case 2: {
			if (const Student *student = findStudent(students); student != nullptr) {
				std::cout << "\n" << GREEN << "Student is found" << RESET << "\n";
				student->print();
				std::cout << "\n";
			}
			break;
		}
		default:
			std::cout << "\nInvalid choice. Please try again." << std::endl;
			break;
		}
	}
}

/**
 * @brief Program entry point
 *
 * Loads students from JSON, prints them, displays a summary with parse time,
 * and enters the interactive management loop.
 *
 * @return 0 on success
 */
int main() {
	const auto start = std::chrono::system_clock::now();

	auto students = loadStudentsFromJson(FILENAME);
	printStudents(students);

	const auto end = std::chrono::system_clock::now();
	printSummary(students, end - start);

	return workWithStudent(students);
}
