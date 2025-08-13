// gradingsystem.h
#ifndef GRADINGSYSTEM_H
#define GRADINGSYSTEM_H

#include <string>
#include <vector>
#include <algorithm> // For std::remove_if, std::find, std::transform
#include <sstream>   // For std::stringstream
#include <fstream>   // For file operations
#include <cctype>    // For isalpha, isdigit, isspace

// Helper functions for validation (can be made static members of GradingSystem or kept global)
// These are adapted from your original code.

/**
 * @brief Checks if a given name string is valid.
 * A name is valid if it's not empty and contains only alphabetic characters or spaces.
 * @param name The string to validate.
 * @return True if the name is valid, false otherwise.
 */
bool isValidName(const std::string &name);

/**
 * @brief Checks if a given phone number string is valid.
 * A phone number is valid if it has exactly 10 digits.
 * @param phone The string to validate.
 * @return True if the phone number is valid, false otherwise.
 */
bool isValidPhone(const std::string &phone);

/**
 * @brief Checks if a given date of birth string is valid and in "dd-mm-yyyy" format.
 * Includes basic date validation (day, month, year ranges, leap years).
 * @param dob The string to validate.
 * @return True if the DOB is valid, false otherwise.
 */
bool isValidDOB(const std::string &dob);

/**
 * @brief Checks if a given grade string is one of the predefined valid grades.
 * @param grade The string to validate.
 * @return True if the grade is valid, false otherwise.
 */
bool isValidGrade(const std::string &grade);

/**
 * @brief Structure to represent a student's data.
 * Contains personal information and a list of grades.
 */
struct Student
{
    std::string name, roll, phone, dob, semester, branch;
    std::vector<std::string> grades; // Stores grades for subjects

    /**
     * @brief Converts a numerical mark into a letter grade.
     * @param mark The mark (0-100).
     * @return The corresponding letter grade string.
     */
    std::string getGrade(int mark)
    {
        if (mark >= 91)
            return "O";
        else if (mark >= 81)
            return "A+";
        else if (mark >= 71)
            return "A";
        else if (mark >= 61)
            return "B+";
        else if (mark >= 51)
            return "B";
        else if (mark >= 41)
            return "C";
        else if (mark >= 31)
            return "D";
        else if (mark >= 21)
            return "E";
        else if (mark >= 11)
            return "P";
        else
            return "F";
    }

    /**
     * @brief Serializes the student object into a CSV formatted string.
     * @return A string representing the student data, comma-separated.
     */
    std::string serialize() const
    {
        std::stringstream ss;
        ss << name << "," << roll << "," << phone << "," << dob << "," << semester << "," << branch;
        for (const std::string &grade : grades)
            ss << "," << grade;
        return ss.str();
    }
};

/**
 * @brief Main class for the grading system logic.
 * Manages admin authentication, student data loading/saving, and CRUD operations.
 */
class GradingSystem
{
private:
    std::string adminEmail;
    std::string adminPass;
    std::vector<Student> students;
    std::string adminFile = "admin.csv";
    std::string selectedSemester;
    std::string selectedBranch;
    std::string targetFile; // CSV file for the currently selected semester/branch

    /**
     * @brief Loads admin credentials from the admin.csv file.
     * If the file doesn't exist, it creates it with default credentials.
     */
    void loadAdmin();

    /**
     * @brief Saves the current list of students to the target CSV file.
     */
    void saveStudents();

    /**
     * @brief Loads student records from the target CSV file into the 'students' vector.
     */
    void loadStudents();

public:
    GradingSystem(); // Constructor

    /**
     * @brief Authenticates an admin user.
     * @param email The email entered by the user.
     * @param password The password entered by the user.
     * @return True if authentication is successful, false otherwise.
     */
    bool login(const std::string &email, const std::string &password);

    /**
     * @brief Validates if a given roll number matches the expected format for a specific branch.
     * Format: 2KYY/BR/NN (e.g., 2K20/CO/001 for Computer branch).
     * @param roll The roll number string to validate.
     * @param branch The branch name (e.g., "computer", "electrical").
     * @return True if the roll number is valid for the given branch, false otherwise.
     */
    bool isValidRollForBranch(const std::string &roll, const std::string &branch);

    /**
     * @brief Sets the current semester and branch, and updates the target CSV file.
     * This will trigger loading the students for the new selection.
     * @param semester The semester string (e.g., "1", "8").
     * @param branch The branch string (e.g., "computer", "electrical").
     */
    void setCurrentSemesterAndBranch(const std::string &semester, const std::string &branch);

    /**
     * @brief Inserts a new student record into the system.
     * @param s The Student object to insert.
     * @return A pair: bool indicating success, and a string message (e.g., error message).
     */
    std::pair<bool, std::string> insertStudent(const Student &s);

    /**
     * @brief Retrieves a student's data by roll number.
     * @param roll The roll number to search for.
     * @param foundStudent Reference to a Student object where the found data will be stored.
     * @return True if the student is found, false otherwise.
     */
    bool viewStudent(const std::string &roll, Student &foundStudent);

    /**
     * @brief Modifies an existing student record.
     * @param oldRoll The roll number of the student to modify.
     * @param newStudent The new Student object with updated data.
     * @return A pair: bool indicating success, and a string message.
     */
    std::pair<bool, std::string> modifyStudent(const std::string &oldRoll, const Student &newStudent);

    /**
     * @brief Deletes a student record by roll number.
     * @param roll The roll number of the student to delete.
     * @return A pair: bool indicating success, and a string message.
     */
    std::pair<bool, std::string> deleteStudent(const std::string &roll);

    /**
     * @brief Gets the currently selected semester.
     * @return The selected semester string.
     */
    std::string getSelectedSemester() const { return selectedSemester; }

    /**
     * @brief Gets the currently selected branch.
     * @return The selected branch string.
     */
    std::string getSelectedBranch() const { return selectedBranch; }
};

#endif // GRADINGSYSTEM_H
