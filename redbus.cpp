#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cctype>

using namespace std;

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

void saveUserToCSV(const string& phoneNumber) {
    ofstream file("Users.csv", ios::app); // Append mode
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

bool isValidPhoneNumber(const string& phone) {
    if (phone.length() != 10) return false;
    for (char c : phone) {
        if (!isdigit(c)) return false;
    }
    return true;
}

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
            continue; // ask again
        }

        // Generate random 4-digit OTP
        srand(time(0));
        int otp = rand() % 9000 + 1000;

        cout << "Generating OTP...\n";
        cout << "Your OTP is: " << otp << endl;

        if (verifyOTP(otp)) {
            saveUserToCSV(phoneNumber);
            return true; // success
        } else {
            if (!getYesNo("Do you want to try again? (Y/N): ")) {
                return false; // user chose not to retry
            }
        }
    }
}

void dashboard() {
    cout << "Welcome to the Dashboard!\n";
    // Placeholder for future functionality
}

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
