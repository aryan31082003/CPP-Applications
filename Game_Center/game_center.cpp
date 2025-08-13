#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <windows.h>
using namespace std;
// aryan gahlaut 2k22/co/102
class User
{
public:
    string userId;
    string email;
    string password;
    double totalCharges;
    string lastGamePlayed;

    User(const string &id, const string &email, const string &pass, double charges = 0.0, const string &lastGame = "")
        : userId(id), email(email), password(pass), totalCharges(charges), lastGamePlayed(lastGame) {}

    void update_charge(double charges)
    {
        totalCharges += charges;
    }
    // aryan gahlaut 2k22/co/102
    void update_last_game(const string &game)
    {
        lastGamePlayed = game;
    }

    string write_to_csv() const
    {
        stringstream ss;
        ss << userId << "," << email << "," << password << "," << fixed << setprecision(2) << totalCharges << "," << lastGamePlayed;
        return ss.str();
    }

    static User Read_from_csv(const string &line)
    {
        stringstream ss(line);
        string id, email, pass, lastGame;
        double charges;
        getline(ss, id, ','); // aryan gahlaut 2k22/co/102
        getline(ss, email, ',');
        getline(ss, pass, ',');
        ss >> charges;
        ss.ignore(1); // Skip comma
        getline(ss, lastGame);
        return User(id, email, pass, charges, lastGame);
    }

    void displayInfo() const
    {
        cout << endl;
        cout << endl;
        cout << "User ID: " << userId << "\n"
             << "Email: " << email << "\n"
             << "Total Charges: $" << fixed << setprecision(2) << totalCharges << "\n"
             << "Last Game Played: " << (lastGamePlayed.empty() ? "None" : lastGamePlayed) << "\n";
        cout << endl;
        cout << endl;
    } // aryan gahlaut 2k22/co/102
};

class GMC
{
private:
    vector<string> games;
    vector<User> users;
    string csvname;

public:
    GMC(const string &csvFile) : csvname(csvFile)
    {
        loadUsers();
        games = {"God of War", "The last of us part 1", "Marvel spiderman 2"};
    }
    // aryan gahlaut 2k22/co/102
    void loadUsers()
    {
        ifstream file(csvname);
        string line;
        while (getline(file, line))
        {
            users.push_back(User::Read_from_csv(line));
        }
    }

    void saveUsers()
    {
        ofstream file(csvname);
        for (const auto &user : users)
        {
            file << user.write_to_csv() << "\n";
        } // aryan gahlaut 2k22/co/102
    }

    void show_game() const
    {
        cout << "Available games:\n";
        for (size_t i = 0; i < games.size(); ++i)
        {
            cout << i + 1 << ". " << games[i] << "\n";
        }
    }

    void play_game(User &user)
    {
        show_game();
        int choice;
        cout << "Select a game by number: ";
        cin >> choice;

        if (choice < 1 || choice > games.size())
        { // aryan gahlaut 2k22/co/102
            cout << "Invalid choice. No game selected.\n";
            return;
        }

        int hours;
        cout << "Enter hours to play: ";
        cin >> hours;

        double rate = 10.0;
        double charges = hours * rate;
        user.update_charge(charges);
        user.update_last_game(games[choice - 1]);

        cout << "You played " << games[choice - 1] << " for " << hours << " hours. Charges: $" << charges << "\n";
    }

    User *finding_user(const string &id)
    {
        for (auto &user : users)
        {
            if (user.userId == id)
            {
                return &user; // aryan gahlaut 2k22/co/102
            }
        }
        return nullptr;
    }

    void adding_user(const User &newUser)
    {
        users.push_back(newUser);
    }

    void display_info(const string &userId)
    {
        User *user = finding_user(userId);
        if (user)
        {
            user->displayInfo();
        }
        else
        {
            cout << "User not found.\n";
        }
    }

    void start()
    { // aryan gahlaut 2k22/co/102
        string userId, email, password;

        cout << "Enter User ID: ";
        cin >> userId;
        User *user = finding_user(userId);

        if (user)
        {
            cout << "Enter Password: ";
            cin >> password;
            if (user->password == password)
            {
                cout << "Welcome back, " << user->email << "!\n";
            }
            else
            {

                Beep(4000, 400);
                Beep(4000, 400);
                Beep(4000, 400);
                Beep(4000, 400);
                cout << "Incorrect password!\n";
                return;
            }
        }
        else
        {
            cout << "New user detected!\nEnter Email: ";
            cin >> email;
            cout << "Enter Password: ";
            cin >> password;
            adding_user(User(userId, email, password));
            user = finding_user(userId);
            cout << "User created successfully!\n"; // aryan gahlaut 2k22/co/102
        }

        bool continuePlaying = true;
        while (continuePlaying)
        {
            char viewInfo;
            cout << "Do you want to see your information? (y/n): ";
            cin >> viewInfo;
            if (viewInfo == 'y' || viewInfo == 'Y')
            {
                user->displayInfo();
            }

            play_game(*user);

            char playMore;
            cout << "Do you want to play another game? (y/n): ";
            cin >> playMore;
            continuePlaying = (playMore == 'y' || playMore == 'Y');
        }

        saveUsers();
    }
};

int main()
{ // aryan gahlaut 2k22/co/102
    GMC center("user_data.csv");
    center.start();
    return 0;
}
