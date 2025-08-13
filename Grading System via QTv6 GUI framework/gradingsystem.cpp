// gradingsystem.cpp
#include "gradingsystem.h"
#include <iostream> // For debugging purposes, can be removed in final GUI app

// Global helper functions implementation
bool isValidName(const std::string &name)
{
    return !name.empty() && std::all_of(name.begin(), name.end(), [](char c) {
        return std::isalpha(c) || std::isspace(c);
    });
}

bool isValidPhone(const std::string &phone)
{
    return phone.length() == 10 && std::all_of(phone.begin(), phone.end(), ::isdigit);
}

bool isValidDOB(const std::string &dob)
{
    if (dob.size() != 10 || dob[2] != '-' || dob[5] != '-')
        return false;
    int day, month, year;
    try
    {
        day = std::stoi(dob.substr(0, 2));
        month = std::stoi(dob.substr(3, 2));
        year = std::stoi(dob.substr(6, 4));
    }
    catch (...)
    {
        return false;
    }
    if (year < 1900 || year > 2100 || month < 1 || month > 12 || day < 1)
        return false;
    std::vector<int> daysInMonth = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
        daysInMonth[1] = 29; // Leap year
    return day <= daysInMonth[month - 1];
}

bool isValidGrade(const std::string &grade)
{
    std::vector<std::string> validGrades = {"O", "A+", "A", "B+", "B", "C", "D", "E", "P", "F"};
    return std::find(validGrades.begin(), validGrades.end(), grade) != validGrades.end();
}

// GradingSystem class implementation
GradingSystem::GradingSystem()
{
    loadAdmin();
}

void GradingSystem::loadAdmin()
{
    std::ifstream file(adminFile);
    if (!file.is_open())
    {
        // If admin file doesn't exist, create it with default credentials
        std::ofstream newFile(adminFile);
        if (newFile.is_open())
        {
            newFile << "pc114335@gmail.com,tokyo123\n";
            newFile.close();
            // std::cout << "Admin file not found. Created with default admin.\n"; // For debugging
        }
        else
        {
            // std::cerr << "Error: Could not create admin file.\n"; // For debugging
        }
    }
    file.close(); // Close the first attempt to open

    file.open(adminFile); // Re-open to read, now it should exist
    std::string line;
    if (file.is_open() && std::getline(file, line))
    {
        std::stringstream ss(line);
        std::getline(ss, adminEmail, ',');
        std::getline(ss, adminPass);
    }
    file.close();
}

bool GradingSystem::login(const std::string &email, const std::string &password)
{
    return (email == adminEmail && password == adminPass);
}

void GradingSystem::loadStudents()
{
    students.clear(); // Clear existing students before loading
    std::ifstream file(targetFile);
    if (!file.is_open())
    {
        // File might not exist yet for a new semester/branch, which is fine.
        return;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        Student s;
        std::string grade;

        // Read fixed fields
        std::getline(ss, s.name, ',');
        std::getline(ss, s.roll, ',');
        std::getline(ss, s.phone, ',');
        std::getline(ss, s.dob, ',');
        std::getline(ss, s.semester, ',');
        std::getline(ss, s.branch, ',');

        // Read grades (variable number)
        while (std::getline(ss, grade, ','))
        {
            s.grades.push_back(grade);
        }
        students.push_back(s);
    }
    file.close();
}

void GradingSystem::saveStudents()
{
    std::ofstream file(targetFile);
    if (!file.is_open())
    {
        // std::cerr << "Error: Could not open " << targetFile << " for writing.\n"; // For debugging
        return;
    }
    for (const Student &s : students) // Use const reference here
    {
        file << s.serialize() << "\n";
    }
    file.close();
}

bool GradingSystem::isValidRollForBranch(const std::string &roll, const std::string &branch)
{
    std::string code;
    // Map branch names to their roll number codes
    if (branch == "computer")
        code = "CO";
    else if (branch == "electrical")
        code = "EE";
    else if (branch == "mechanical")
        code = "MC";
    else if (branch == "chemical")
        code = "CH";
    else if (branch == "civil")
        code = "CV";
    else if (branch == "management")
        code = "MB";
    else
        return false; // Unknown branch

    if (roll.length() < 9) return false; // Minimum length check, e.g., 2K20/CO/01
    if (roll.substr(0, 2) != "2K")
        return false;

    size_t firstSlash = roll.find('/');
    size_t secondSlash = roll.find('/', firstSlash + 1);
    if (firstSlash == std::string::npos || secondSlash == std::string::npos)
        return false;

    std::string year = roll.substr(2, firstSlash - 2);
    std::string branchCode = roll.substr(firstSlash + 1, secondSlash - firstSlash - 1);
    std::string serial = roll.substr(secondSlash + 1);

    if (branchCode != code)
        return false; // Branch code mismatch

    // Validate that year and serial parts are numeric
    if (year.empty() || serial.empty())
        return false;
    if (!std::all_of(year.begin(), year.end(), ::isdigit))
        return false;
    if (!std::all_of(serial.begin(), serial.end(), ::isdigit))
        return false;

    return true;
}

void GradingSystem::setCurrentSemesterAndBranch(const std::string &semester, const std::string &branch)
{
    selectedSemester = semester;
    selectedBranch = branch;
    targetFile = selectedBranch + "_" + selectedSemester + ".csv";
    loadStudents(); // Load students specific to this semester and branch
}

std::pair<bool, std::string> GradingSystem::insertStudent(const Student &s)
{
    // Check for duplicate roll number
    for (const auto &existingStudent : students)
    {
        if (existingStudent.roll == s.roll)
        {
            return {false, "Error: Student with this roll number already exists."};
        }
    }

    students.push_back(s);
    saveStudents();
    return {true, "Student added successfully."};
}

bool GradingSystem::viewStudent(const std::string &roll, Student &foundStudent)
{
    for (const auto &s : students) // Use const reference here
    {
        if (s.roll == roll)
        {
            foundStudent = s; // Copy the found student data
            return true;
        }
    }
    return false; // Student not found
}

std::pair<bool, std::string> GradingSystem::modifyStudent(const std::string &oldRoll, const Student &newStudent)
{
    auto it = std::find_if(students.begin(), students.end(),
                           [&](const Student &s) { return s.roll == oldRoll; });

    if (it != students.end())
    {
        // Check if the newRoll is different from oldRoll and if it already exists
        if (oldRoll != newStudent.roll) {
            for (const auto &existingStudent : students) {
                if (existingStudent.roll == newStudent.roll && existingStudent.roll != oldRoll) {
                    return {false, "Error: New roll number already exists for another student."};
                }
            }
        }
        *it = newStudent; // Update the student data
        saveStudents();
        return {true, "Student data modified successfully."};
    }
    else
    {
        return {false, "Error: Student not found."};
    }
}

std::pair<bool, std::string> GradingSystem::deleteStudent(const std::string &roll)
{
    auto it = std::remove_if(students.begin(), students.end(),
                             [&](const Student &s) { return s.roll == roll; });

    if (it != students.end())
    {
        students.erase(it, students.end());
        saveStudents();
        return {true, "Student record deleted successfully."};
    }
    else
    {
        return {false, "Error: Student not found."};
    }
}
