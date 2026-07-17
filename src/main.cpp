#include "InputReader.h"
#include "Menus.h"
#include "Student.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <optional>
#include <simdjson.h>
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

	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cout << "Could not open " << filename << std::endl;
		return students;
	}

	std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();

	simdjson::dom::parser parser;
	simdjson::dom::element json = parser.parse(contents);

	for (const auto &item : json.get_array()) {
		try {
			Student s;

			s.setName(std::string(item["name"].get_string().value()));
			s.setAge(static_cast<int>(item["age"].get_int64().value()));
			s.setActivity(static_cast<Activity>(item["activity"].get_int64().value()));
			s.setId(static_cast<int>(item["id"].get_int64().value()));

			students.push_back(std::move(s));
		} catch (const std::exception &e) {
			continue;
		}
	}

	std::cout << "\nJSON is Loaded. Length: " << GREEN << students.size() << RESET << "\n" << std::endl;

	return students;
}

/**
 * @brief Print all students to stdout
 *
 * @param students vector of students to print
 */
void printStudents(const std::vector<Student> &students) {
	for (int i = 0; i < students.size(); ++i) {
		if (i % 1000 == 0 || i == students.size() - 1) {
			students[i].print();
		}
	}

	std::cout << "\n"
	          << "=== END ===" << "\n";
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
	std::cout << "- Study: " << std::ranges::count_if(students.begin(), students.end(), [](const Student &s) {
		return s.getActivity() == STUDY;
	}) << std::endl;
	std::cout << "- Exercise: " << std::ranges::count_if(students.begin(), students.end(), [](const Student &s) {
		return s.getActivity() == EXERCISE;
	}) << "\n" << std::endl;
}

int getMaxId(const std::vector<Student> &students) {
	if (students.empty())
		return 0;

	return std::ranges::max_element(students, [](const Student &a, const Student &b) { return a.getId() < b.getId(); })
	    ->getId();
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

	const int maxId = getMaxId(students);
	if (maxId == 0) {
		std::cout << "No students available." << std::endl;
		return nullptr;
	}

	const int id = readIntInRange("Enter student ID", 1, maxId);

	for (auto &s : students) {
		if (s.getId() == id)
			return &s;
	}

	std::cout << "\n" << RED << "Student with ID " << id << " not found." << RESET << "\n";
	return nullptr;
}

bool deleteStudent(std::vector<Student> &students) {
	if (students.empty()) {
		std::cout << "No students available to delete." << std::endl;
		return false;
	}

	const int maxId = getMaxId(students);
	if (maxId == 0) {
		std::cout << "No students available." << std::endl;
		return false;
	}

	int id = readIntInRange("Enter student ID to delete", 1, maxId);

	if (const auto it = std::ranges::find_if(students, [id](const Student &s) { return s.getId() == id; });
	    it != students.end()) {
		students.erase(it);
		std::cout << "Student with ID " << id << " deleted." << std::endl;
		return true;
	}

	std::cout << "\n" << RED << "Student with ID " << id << " not found." << RESET << "\n";
	return false;
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
		case 3: {
			deleteStudent(students);
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
