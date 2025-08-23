#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <vector>
#include <sstream>
#include <algorithm>
#include <ctime>

using namespace std;

// ---------- Struct ----------
struct Bus {
    int id;
    string source;
    string destination;
    string busType;
    int price;
};

// ---------- Utility Functions ----------

// Get yes/no input (Y/N, case insensitive)
bool getYesNo(const string& prompt) {
    char input;
    cout << prompt;
    cin >> input;

    while (input != 'Y' && input != 'y' && input != 'N' && input != 'n') {
        cout << "Invalid input. Please enter Y or N: ";
        cin >> input;
    }

    return (input == 'Y' || input == 'y');
}

// Convert string to lowercase for case-insensitive comparison
string toLower(const string& s) {
    string result = s;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

// Validate phone number: exactly 10 digits
bool isValidPhoneNumber(const string& phone) {
    if (phone.length() != 10) return false;
    for (char c : phone) {
        if (!isdigit(c)) return false;
    }
    return true;
}

// Check if phone number exists in Users.csv
bool isPhoneNumberExists(const string& phoneNumber) {
    ifstream file("Users.csv");
    if (!file.is_open()) {
        // If file doesn't exist, treat as no users yet
        return false;
    }

    string line;
    while (getline(file, line)) {
        if (line == phoneNumber) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

// Save phone number to Users.csv
void saveUserToCSV(const string& phoneNumber) {
    ofstream file("Users.csv", ios::app);
    if (file.is_open()) {
        file << phoneNumber << endl;
        file.close();
        cout << "Phone number saved to Users.csv.\n";
    } else {
        cout << "Failed to open Users.csv.\n";
    }
}

// Verify OTP entered by user
bool verifyOTP(int generatedOTP) {
    int enteredOTP;
    cout << "Enter the OTP sent to your phone: ";
    cin >> enteredOTP;

    if (enteredOTP == generatedOTP) {
        cout << "OTP Verified Successfully.\n";
        return true;
    } else {
        cout << "Incorrect OTP. Verification Failed.\n";
        return false;
    }
}

// ---------- Login/Register Process ----------
bool processLoginOrRegister() {
    char choice;
    cout << "Do you want to Login or Register? (L/R): ";
    cin >> choice;

    if (choice != 'L' && choice != 'l' && choice != 'R' && choice != 'r') {
        cout << "Invalid choice. Try again.\n";
        return processLoginOrRegister(); // retry
    }

    while (true) {
        string phoneNumber;
        cout << "Enter your phone number (10 digits): ";
        cin >> phoneNumber;

        if (!isValidPhoneNumber(phoneNumber)) {
            cout << "Invalid phone number. Must be exactly 10 digits.\n";
            continue;
        }

        if (choice == 'R' || choice == 'r') {
            // For Register, check if phone exists
            if (isPhoneNumberExists(phoneNumber)) {
                cout << "Phone number already exists.\n";
            }
        } else {
            // For Login, no need to check existence message or saving
        }

        srand(time(0));
        int otp = rand() % 9000 + 1000;  // 4-digit OTP

        cout << "Generating OTP...\n";
        cout << "Your OTP is: " << otp << endl;

        if (verifyOTP(otp)) {
            if ((choice == 'R' || choice == 'r') && !isPhoneNumberExists(phoneNumber)) {
                saveUserToCSV(phoneNumber);
            }
            return true;
        } else {
            if (!getYesNo("Do you want to try again? (Y/N): ")) {
                return false;
            }
        }
    }
}

#include <ctime>    // for system time

bool isLeapYear(int year) {
    return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
}

bool isValidDate(const string& dateStr) {
    if (dateStr.length() != 10) return false;
    if (dateStr[2] != '-' || dateStr[5] != '-') return false;

    int day, month, year;
    try {
        day = stoi(dateStr.substr(0, 2));
        month = stoi(dateStr.substr(3, 2));
        year = stoi(dateStr.substr(6, 4));
    } catch (...) {
        return false;
    }

    if (year < 1900 || year > 2100) return false;  // arbitrary limits
    if (month < 1 || month > 12) return false;

    // Days in month accounting leap year for February
    int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (month == 2 && isLeapYear(year)) {
        daysInMonth[1] = 29;
    }

    if (day < 1 || day > daysInMonth[month - 1]) return false;

    // Check if date is not in the future
    time_t t = time(nullptr);
    tm* now = localtime(&t);

    int currYear = now->tm_year + 1900;
    int currMonth = now->tm_mon + 1;
    int currDay = now->tm_mday;

    if (year > currYear) return false;
    if (year == currYear && month > currMonth) return false;
    if (year == currYear && month == currMonth && day > currDay) return false;

    return true;
}

// Update inputs function to validate date
void inputs(string &source, string &destination, string &date) {
    cout << "Enter Source: ";
    cin >> source;
    cout << "Enter Destination: ";
    cin >> destination;

    while (true) {
        cout << "Enter Date (DD-MM-YYYY): ";
        cin >> date;

        if (isValidDate(date)) {
            break;
        } else {
            cout << "Invalid date.Please try again.\n";
        }
    }
}

// ---------- Dashboard Placeholder Functions ----------
void searchBus() {
    string source, destination, date;
    inputs(source, destination, date);

    string sourceLower = toLower(source);
    string destinationLower = toLower(destination);

    ifstream file("Buses.csv");
    if (!file.is_open()) {
        cout << "Could not open Buses.csv file.\n";
        return;
    }

    vector<Bus> matchedBuses;
    string line;

    while (getline(file, line)) {
        if (line.empty()) continue;  // skip blank lines

        stringstream ss(line);
        string idStr, src, dest, type, priceStr;

        getline(ss, idStr, ',');
        getline(ss, src, ',');
        getline(ss, dest, ',');
        getline(ss, type, ',');
        getline(ss, priceStr, ',');

        string srcLower = toLower(src);
        string destLower = toLower(dest);

        if (srcLower == sourceLower && destLower == destinationLower) {
            Bus bus;
            bus.id = stoi(idStr);
            bus.source = src;
            bus.destination = dest;
            bus.busType = type;
            bus.price = stoi(priceStr);
            matchedBuses.push_back(bus);
        }
    }
    file.close();

    if (matchedBuses.empty()) {
        cout << "No buses found for " << source << " to " << destination << ".\n";
        return;
    }

    cout << "\nAvailable buses from " << source << " to " << destination << " on " << date << ":\n";
    cout << "ID\tSource\t\tDestination\tType\t\tPrice\n";

    for (size_t i = 0; i < matchedBuses.size(); i++) {
        cout << (i + 1) << "\t" << matchedBuses[i].source << "\t\t" << matchedBuses[i].destination
             << "\t\t" << matchedBuses[i].busType << "\t\t" << matchedBuses[i].price << "\n";
    }

    int choice;
    cout << "Enter bus ID to choose: ";
    cin >> choice;

    if (choice < 1 || choice >(int)matchedBuses.size()) {
        cout << "Invalid bus ID selected.\n";
        return;
    }

    cout << "You selected Bus ID: " << choice << " (" << matchedBuses[choice - 1].busType << ")\n";
    cout << "Proceeding to seat selection (functionality to be implemented)...\n";
}

void bookings() {
    cout << "Bookings functionality to be implemented...\n";
}

void updateProfile() {
    cout << "Update profile functionality to be implemented...\n";
}

// ---------- Dashboard Function ----------
void dashboard() {
    bool stayInDashboard = true;

    while (stayInDashboard) {
        cout << "\n--- DASHBOARD ---\n";
        cout << "1. Search Bus\n";
        cout << "2. Bookings\n";
        cout << "3. Update Profile\n";
        cout << "Enter your choice (1-3): ";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                searchBus();
                break;
            case 2:
                bookings();
                break;
            case 3:
                updateProfile();
                break;
            default:
                cout << "Invalid choice. Please enter 1, 2, or 3.\n";
                continue; // show menu again
        }

        stayInDashboard = getYesNo("Go back to dashboard? (Y/N): ");
    }
}

// ---------- Main ----------
int main() {
    if (processLoginOrRegister()) {
        dashboard();

        if (getYesNo("Do you want to logout? (Y/N): ")) {
            cout << "Logout successfully.\n";
        }
    } else {
        cout << "Returning to main menu. Login/Register not completed.\n";
    }

    return 0;
}
