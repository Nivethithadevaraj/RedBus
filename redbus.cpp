#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cctype>

using namespace std;

// ---------- Utility Functions ----------
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

bool isValidPhoneNumber(const string& phone) {
    if (phone.length() != 10) return false;
    for (char c : phone) {
        if (!isdigit(c)) return false;
    }
    return true;
}

// *** Added: Check if phone number exists in Users.csv ***
bool isPhoneNumberExists(const string& phoneNumber) {
    ifstream file("Users.csv");
    string line;
    while (getline(file, line)) {
        if (line == phoneNumber) {
            return true;
        }
    }
    return false;
}

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

    string phoneNumber;

    while (true) {
        cout << "Enter your phone number (10 digits): ";
        cin >> phoneNumber;

        if (!isValidPhoneNumber(phoneNumber)) {
            cout << "Invalid phone number. Must be exactly 10 digits.\n";
            continue;
        }
        break;
    }

    if (isPhoneNumberExists(phoneNumber)) {
        cout << "Phone number already exists.\n";
    }

    while (true) {
        srand(time(0));
        int otp = rand() % 9000 + 1000;

        cout << "Generating OTP...\n";
        cout << "Your OTP is: " << otp << endl;

        if (verifyOTP(otp)) {
            if (!isPhoneNumberExists(phoneNumber)) {
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


// ---------- Dashboard Placeholder Functions ----------
void searchBus() {
    // To be implemented later
}

void bookings() {
    // To be implemented later
}

void updateProfile() {
    // To be implemented later
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
