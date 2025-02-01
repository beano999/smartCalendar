#include <iostream>
#include <iomanip>
#include <ctime>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

// Function prototypes
int getDaysInMonth(int month, int year);
int getStartingDay(int month, int year);
void printCalendar(int daysInMonth, int startingDay);
void printCalendar(int daysInMonth, int startingDay, const map<int, vector<string>>& events);
void addEvent(map<int, vector<string>>& events, int currentMonth, int currentYear);
int eventHandler(const string& input, map<int, vector<string>>& events, int currentMonth, int currentYear, int daysInMonth, int startingDay);
int aiHandler(map<int, vector<string>>& events, int currentMonth, int currentYear, int daysInMonth, int startingDay);

int main() {
    // Get the current date
    time_t t = time(0);
    struct tm now;
    localtime_s(&now, &t);
    int currentMonth = now.tm_mon + 1; // tm_mon is 0-11
    int currentYear = now.tm_year + 1900; // tm_year is years since 1900

    // Get the number of days in the current month and the starting day
    int daysInMonth = getDaysInMonth(currentMonth, currentYear);
    int startingDay = getStartingDay(currentMonth, currentYear);

    // Events map to store events by date
    map<int, vector<string>> events;

    string input;
    int command = 1;

    cout << "Welcome to Smart Calendar!" << endl;
    while (command != 0) {
        cout << "Enter a command (help for commands): ";
        getline(cin, input);
        command = eventHandler(input, events, currentMonth, currentYear, daysInMonth, startingDay);
    }

    return 0;
}

// Function to get the number of days in a month
int getDaysInMonth(int month, int year) {
    switch (month) {
    case 1: case 3: case 5: case 7: case 8: case 10: case 12:
        return 31;
    case 4: case 6: case 9: case 11:
        return 30;
    case 2:
        // Check for leap year
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
            return 29;
        else
            return 28;
    default:
        return 0;
    }
}

// Function to get the starting day of the week for a given month and year
int getStartingDay(int month, int year) {
    tm time_in = { 0, 0, 0, 1, month - 1, year - 1900 }; // Month is 0-11 in struct tm
    mktime(&time_in);
    return time_in.tm_wday;
}

// Overloaded function to print the calendar without events
void printCalendar(int daysInMonth, int startingDay) {
    int day = 1;
    for (int i = 0; i < 6; i++) { // 6 weeks max in a month
        for (int j = 0; j < 7; j++) {
            if (i == 0 && j < startingDay) {
                cout << "   "; // Empty space for days before the start of the month
            }
            else if (day <= daysInMonth) {
                cout << setw(2) << day << " ";
                day++;
            }
        }
        cout << endl;
    }
}

// Overloaded function to print the calendar with events
void printCalendar(int daysInMonth, int startingDay, const map<int, vector<string>>& events) {
    int day = 1;
    for (int i = 0; i < 6; i++) { // 6 weeks max in a month
        for (int j = 0; j < 7; j++) {
            if (i == 0 && j < startingDay) {
                cout << "   "; // Empty space for days before the start of the month
            }
            else if (day <= daysInMonth) {
                if (events.find(day) != events.end()) {
                    cout << "\033[1;31m" << setw(2) << day << "\033[0m "; // Red color
                }
                else {
                    cout << setw(2) << day << " ";
                }
                day++;
            }
        }
        cout << endl;
    }

    // Print events below the calendar
    for (const auto& event : events) {
        cout << "Day " << event.first << ":" << endl;
        for (const auto& name : event.second) {
            cout << "  - " << name << endl;
        }
    }
}

void addEvent(map<int, vector<string>>& events, int currentMonth, int currentYear) {
    int date = 0;
    string eventName;
    char choice;

    do {
        cout << "Enter the day number of the event (1-" << getDaysInMonth(currentMonth, currentYear) << "): ";
        cin >> date;
    } while (date < 1 || date > getDaysInMonth(currentMonth, currentYear));
    cout << "Enter the event name: ";
    cin.ignore();  // Ignore the newline character left in the buffer
    getline(cin, eventName);

    cout << "Add description? (y/n): ";
    cin >> choice;
    choice = toupper(choice);
    if (choice == 'Y') {
        string description;
        cout << "Enter the description: ";
        cin.ignore();  // Ignore the newline character left in the buffer
        getline(cin, description);
        eventName += " - " + description;
    }

    events[date].push_back(eventName);
    cout << "Event added!" << endl;
}

// Event Handler
int eventHandler(const string& input, map<int, vector<string>>& events, int currentMonth, int currentYear, int daysInMonth, int startingDay) {
    time_t t = time(0);
    struct tm now;
    localtime_s(&now, &t);

    if (input == "help") {
        cout << "Commands: " << endl;
        cout << "add - Add an event" << endl;
        cout << "view - View the calendar without events" << endl;
        cout << "view events - View the calendar with events" << endl;
        cout << "exit - Exit the program" << endl;
        cout << "ai - Activate AI" << endl;
        return 1;
    }
    else if (input == "add") {
        char choice;
        do {
            addEvent(events, currentMonth, currentYear);
            cout << "Do you want to add another event? (y/n): ";
            cin >> choice;
            choice = toupper(choice);
            cin.ignore();  // Ignore the newline character left in the buffer
        } while (choice == 'Y');
        return 2;
    }
    else if (input == "view events") {
        // Print the calendar with events
        cout << "     " << put_time(&now, "%B") << " " << currentYear << endl;
        cout << "Su Mo Tu We Th Fr Sa" << endl;
        printCalendar(daysInMonth, startingDay, events);
        return 3;
    }
    else if (input == "view") {
        // Print the calendar without events
        cout << "     " << put_time(&now, "%B") << " " << currentYear << endl;
        cout << "Su Mo Tu We Th Fr Sa" << endl;
        printCalendar(daysInMonth, startingDay);
        return 4;
    }
    else if (input == "ai") {
        aiHandler(events, currentMonth, currentYear, daysInMonth, startingDay);
        return 5;
    }
    else if (input == "exit") {
        return 0;
    }
    else {
        cout << "Invalid command. Enter 'help' for a list of commands." << endl;
        return 1;
    }
}

// AI Handler function
int aiHandler(map<int, vector<string>>& events, int currentMonth, int currentYear, int daysInMonth, int startingDay) {
    string userInput;
    int length = 0;
    int wordCount = 0;
    vector<string> words;

    cout << "AI activated!" << endl;
    cout << "Enter what you want to do: ";
    getline(cin, userInput);
    transform(userInput.begin(), userInput.end(), userInput.begin(), ::toupper);

    stringstream ss(userInput);
    string word;
    while (ss >> word) {
        words.push_back(word);
        wordCount++;
        length += word.length();
    }

    // Keyword handling
    if (find(words.begin(), words.end(), "ADD") != words.end() || find(words.begin(), words.end(), "CREATE") != words.end()) {
        addEvent(events, currentMonth, currentYear);
    }
    else if (find(words.begin(), words.end(), "VIEW") != words.end() && find(words.begin(), words.end(), "EVENTS") != words.end()) {
        time_t t = time(0);
        struct tm now;
        localtime_s(&now, &t);
        cout << "     " << put_time(&now, "%B") << " " << currentYear << endl;
        cout << "Su Mo Tu We Th Fr Sa" << endl;
        printCalendar(daysInMonth, startingDay, events);
    }
    else if (find(words.begin(), words.end(), "VIEW") != words.end()) {
        time_t t = time(0);
        struct tm now;
        localtime_s(&now, &t);
        cout << "     " << put_time(&now, "%B") << " " << currentYear << endl;
        cout << "Su Mo Tu We Th Fr Sa" << endl;
        printCalendar(daysInMonth, startingDay);
    }
    else if (find(words.begin(), words.end(), "HELP") != words.end()) {
        cout << "Commands: " << endl;
        cout << "add - Add an event" << endl;
        cout << "view - View the calendar without events" << endl;
        cout << "view events - View the calendar with events" << endl;
        cout << "exit - Exit the program" << endl;
        cout << "ai - Activate AI" << endl;
    }
    else {
        cout << "No recognized command found in AI input." << endl;
    }

    return 1;
}
