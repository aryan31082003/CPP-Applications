

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <cstdlib>

using namespace std;

bool isValidName(const string &name)
{
    return !name.empty() && all_of(name.begin(), name.end(), [](char c)
                                   { return isalpha(c) || isspace(c); });
}

bool isValidPhone(const string &phone)
{
    return phone.length() == 10 && all_of(phone.begin(), phone.end(), ::isdigit);
}

bool isValidDOB(const string &dob)
{
    if (dob.size() != 10 || dob[2] != '-' || dob[5] != '-')
        return false;
    int day, month, year;
    try
    {
        day = stoi(dob.substr(0, 2));
        month = stoi(dob.substr(3, 2));
        year = stoi(dob.substr(6, 4));
    }
    catch (...)
    {
        return false;
    }
    if (year < 1900 || year > 2100 || month < 1 || month > 12 || day < 1)
        return false;
    vector<int> daysInMonth = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
        daysInMonth[1] = 29;
    return day <= daysInMonth[month - 1];
}

bool isValidGrade(const string &grade)
{
    vector<string> validGrades = {"O", "A+", "A", "B+", "B", "C", "D", "E", "P", "F"};
    return find(validGrades.begin(), validGrades.end(), grade) != validGrades.end();
}

struct Student
{
    string name, roll, phone, dob, semester, branch;
    vector<string> grades;

    string getGrade(int mark)
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

    string serialize()
    {
        stringstream ss;
        ss << name << "," << roll << "," << phone << "," << dob << "," << semester << "," << branch;
        for (const string &grade : grades)
            ss << "," << grade;
        return ss.str();
    }

    void display()
    {
        cout << "\nName: " << name << "\nRoll: " << roll << "\nPhone: " << phone;
        cout << "\nDOB: " << dob << "\nSemester: " << semester << "\nBranch: " << branch << "\nGrades:\n";
        for (size_t i = 0; i < grades.size(); ++i)
            cout << "Subject " << i + 1 << ": " << grades[i] << "\n";
    }
};

class GradingSystem
{
    string adminEmail, adminPass;
    vector<Student> students;
    string adminFile = "admin.csv";
    string selectedSemester, selectedBranch;
    string targetFile;

public:
    bool isValidRollForBranch(const string &roll, const string &branch)
    {
        string code;
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
            return false;

        if (roll.substr(0, 2) != "2K")
            return false;
        size_t firstSlash = roll.find('/');
        size_t secondSlash = roll.find('/', firstSlash + 1);
        if (firstSlash == string::npos || secondSlash == string::npos)
            return false;

        string year = roll.substr(2, firstSlash - 2);
        string branchCode = roll.substr(firstSlash + 1, secondSlash - firstSlash - 1);
        string serial = roll.substr(secondSlash + 1);

        if (branchCode != code)
            return false;
        if (year.empty() || serial.empty())
            return false;
        if (!all_of(year.begin(), year.end(), ::isdigit))
            return false;
        if (!all_of(serial.begin(), serial.end(), ::isdigit))
            return false;

        return true;
    }

    void loadAdmin()
    {
        ifstream file(adminFile);
        if (!file.is_open())
        {
            ofstream newFile(adminFile);
            newFile << "pc114335@gmail.com,tokyo123\n";
            newFile.close();
            cout << "Admin file not found. Created with default admin.\n";
        }

        file.close();
        file.open(adminFile);
        string line;
        if (getline(file, line))
        {
            stringstream ss(line);
            getline(ss, adminEmail, ',');
            getline(ss, adminPass);
        }
        file.close();
    }

    bool login()
    {
        cout << "Enter admin email: ";
        string email, pass;
        cin >> email;
        cout << "Enter password: ";
        cin >> pass;
        return (email == adminEmail && pass == adminPass);
    }

    void loadStudents()
    {
        students.clear();
        ifstream file(targetFile);
        if (!file.is_open())
            return;

        string line;
        while (getline(file, line))
        {
            stringstream ss(line);
            Student s;
            string grade;
            getline(ss, s.name, ',');
            getline(ss, s.roll, ',');
            getline(ss, s.phone, ',');
            getline(ss, s.dob, ',');
            getline(ss, s.semester, ',');
            getline(ss, s.branch, ',');
            while (getline(ss, grade, ','))
            {
                s.grades.push_back(grade);
            }
            students.push_back(s);
        }
        file.close();
    }

    void saveStudents()
    {
        ofstream file(targetFile);
        for (Student &s : students)
            file << s.serialize() << "\n";
        file.close();
    }

    void setSemesterAndBranch()
    {
        while (true)
        {
            cout << "Enter semester (1-8): ";
            cin >> selectedSemester;
            try
            {
                int sem = stoi(selectedSemester);
                if (sem < 1 || sem > 8)
                    throw invalid_argument("Invalid range");
                break;
            }
            catch (...)
            {
                cout << "Invalid semester. Please enter a number between 1 and 8.\n";
            }
        }

        while (true)
        {
            cout << "Enter branch (computer, electrical, mechanical, chemical, civil, management): ";
            cin >> selectedBranch;
            vector<string> validBranches = {
                "computer", "electrical", "mechanical", "chemical", "civil", "management"};
            transform(selectedBranch.begin(), selectedBranch.end(), selectedBranch.begin(), ::tolower);
            if (find(validBranches.begin(), validBranches.end(), selectedBranch) != validBranches.end())
                break;
            cout << "Invalid branch. Allowed: computer, electrical, mechanical, chemical, civil, management.\n";
        }

        targetFile = selectedBranch + "_" + selectedSemester + ".csv";
        loadStudents();
    }

    void insertStudent()
    {
        Student s;
        int subjectCount, mark;
        cin.ignore();

        while (true)
        {
            cout << "Enter student name: ";
            getline(cin, s.name);
            if (isValidName(s.name))
                break;
            cout << "Invalid name. Only alphabets and spaces allowed.\n";
        }

        while (true)
        {
            cout << "Enter roll number (Format: 2KXX/BR/XX): ";
            getline(cin, s.roll);
            if (isValidRollForBranch(s.roll, selectedBranch))
                break;
            cout << "Invalid roll number for branch '" << selectedBranch << "'.\n";
        }

        while (true)
        {
            cout << "Enter phone number: ";
            getline(cin, s.phone);
            if (isValidPhone(s.phone))
                break;
            cout << "Invalid phone number. Must be exactly 10 digits.\n";
        }

        while (true)
        {
            cout << "Enter DOB (dd-mm-yyyy): ";
            getline(cin, s.dob);
            if (isValidDOB(s.dob))
                break;
            cout << "Invalid DOB. Must be valid and in dd-mm-yyyy format.\n";
        }

        s.semester = selectedSemester;
        s.branch = selectedBranch;

        cout << "Enter number of subjects: ";
        cin >> subjectCount;
        s.grades.resize(subjectCount);
        for (int i = 0; i < subjectCount; ++i)
        {
            while (true)
            {
                cout << "Enter marks for Subject " << i + 1 << ": ";
                cin >> mark;
                string grade = s.getGrade(mark);
                if (isValidGrade(grade))
                {
                    s.grades[i] = grade;
                    break;
                }
                cout << "Invalid grade.\n";
            }
        }

        students.push_back(s);
        saveStudents();
        cout << "Student added successfully.\n";
    }

    void viewStudent()
    {
        cout << "Enter roll number: ";
        string roll;
        cin >> roll;
        for (Student &s : students)
        {
            if (s.roll == roll)
            {
                s.display();
                return;
            }
        }
        cout << "Student not found.\n";
    }

    void modifyStudent()
    {
        cout << "Enter roll number to modify: ";
        string roll;
        cin >> roll;

        auto it = find_if(students.begin(), students.end(),
                          [&](Student &s)
                          { return s.roll == roll; });

        if (it != students.end())
        {
            students.erase(it);
            insertStudent(); // reuse insert logic
            cout << "Student data modified successfully.\n";
        }
        else
        {
            cout << "Student not found.\n";
        }
    }

    void deleteStudent()
    {
        cout << "Enter roll number to delete: ";
        string roll;
        cin >> roll;
        auto it = remove_if(students.begin(), students.end(),
                            [&](Student &s)
                            { return s.roll == roll; });

        if (it != students.end())
        {
            students.erase(it, students.end());
            saveStudents();
            cout << "Student record deleted.\n";
        }
        else
        {
            cout << "Student not found.\n";
        }
    }

    void menu()
    {
        loadAdmin();
        if (!login())
        {
            cout << "Authentication Failed!\n";
            return;
        }

        int choice;
        do
        {
            cout << "\n--- Grading System ---\n";
            cout << "1. Insert Student\n2. Modify Student\n3. Delete Student\n4. View Student\n5. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            if (choice >= 1 && choice <= 4)
                setSemesterAndBranch();

            switch (choice)
            {
            case 1:
                insertStudent();
                break;
            case 2:
                modifyStudent();
                break;
            case 3:
                deleteStudent();
                break;
            case 4:
                viewStudent();
                break;
            case 5:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice.\n";
            }
        } while (choice != 5);
    }
};

int main()
{
    GradingSystem system;
    system.menu();

    return 0;
}
