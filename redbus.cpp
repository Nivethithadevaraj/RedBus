#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <vector>
#include <sstream>
#include <algorithm>
#include <limits>

using namespace std;

// ---------- Struct ----------
struct Bus {
    int id;
    string date;        // dd-mm-yyyy
    string source;
    string destination;
    int seats;          // from Buses.csv column "SEATS"
    int price;          // PRICE
};

// ---------- Function Declarations ----------
void passengerInfoAndPayment(const vector<int>& seats, int busId);

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

string toLower(const string& s) {
    string result = s;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

bool isValidPhoneNumber(const string& phone) {
    if (phone.length() != 10) return false;
    for (char c : phone) {
        if (!isdigit(c)) return false;
    }
    return true;
}

bool isPhoneNumberExists(const string& phoneNumber) {
    ifstream file("Users.csv");
    if (!file.is_open()) {
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
        return processLoginOrRegister();
    }

    while (true) {
        string phoneNumber;
        cout << "Enter your phone number (10 digits): ";
        cin >> phoneNumber;

        if (!isValidPhoneNumber(phoneNumber)) {
            cout << "Invalid phone number. Must be exactly 10 digits.\n";
            continue;
        }

        if ((choice == 'R' || choice == 'r') && isPhoneNumberExists(phoneNumber)) {
            cout << "Phone number already exists.\n";
            continue;
        }

        srand(time(0));
        int otp = rand() % 9000 + 1000;
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

    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;

    int daysInMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    if (month == 2 && isLeapYear(year)) {
        daysInMonth[1] = 29;
    }
    if (day < 1 || day > daysInMonth[month - 1]) return false;

    // get today date
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    int currYear = now->tm_year + 1900;
    int currMonth = now->tm_mon + 1;
    int currDay = now->tm_mday;

    // allow today & future only
    if (year < currYear) return false;
    if (year == currYear && month < currMonth) return false;
    if (year == currYear && month == currMonth && day < currDay) return false;

    return true;
}

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
            cout << "Invalid date. Please try again.\n";
        }
    }
}

// ---------- Seat Booking ----------
vector<int> loadBookedSeats(int busId) {
    vector<int> bookedSeats;
    ifstream file("Seats.csv");
    if (!file.is_open()) return bookedSeats;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string idStr;
        getline(ss, idStr, ',');
        int id = stoi(idStr);
        if (id == busId) {
            string seatStr;
            while (getline(ss, seatStr, ',')) {
                try {
                    int seatNum = stoi(seatStr);
                    bookedSeats.push_back(seatNum);
                } catch (...) {}
            }
            break;
        }
    }
    file.close();
    return bookedSeats;
}

void saveBookedSeats(int busId, const vector<int>& bookedSeats) {
    ifstream fileIn("Seats.csv");
    vector<string> lines;
    bool busFound = false;

    if (fileIn.is_open()) {
        string line;
        while (getline(fileIn, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string idStr;
            getline(ss, idStr, ',');
            int id = stoi(idStr);

            if (id == busId) {
                busFound = true;
                string newLine = to_string(busId);
                for (int seat : bookedSeats) {
                    newLine += "," + to_string(seat);
                }
                lines.push_back(newLine);
            } else {
                lines.push_back(line);
            }
        }
        fileIn.close();
    }

    if (!busFound) {
        string newLine = to_string(busId);
        for (int seat : bookedSeats) {
            newLine += "," + to_string(seat);
        }
        lines.push_back(newLine);
    }

    ofstream fileOut("Seats.csv", ios::trunc);
    for (const auto& l : lines) {
        fileOut << l << endl;
    }
    fileOut.close();
}

void selectSeat(int busId) {
    vector<int> bookedSeats = loadBookedSeats(busId);
    const int totalSeats = 20;
    vector<int> availableSeats;

    for (int seat = 1; seat <= totalSeats; ++seat) {
        if (find(bookedSeats.begin(), bookedSeats.end(), seat) == bookedSeats.end()) {
            availableSeats.push_back(seat);
        }
    }

    if (availableSeats.empty()) {
        cout << "Sorry, all seats are booked for this bus.\n";
        return;
    }

    cout << "\nAvailable seats: ";
    for (int seat : availableSeats) {
        cout << seat << " ";
    }
    cout << endl;

    cout << "Enter seat numbers to book (space separated): ";
    vector<int> selectedSeats;
    int seatChoice;
    string line;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, line);
    stringstream ss(line);

    while (ss >> seatChoice) {
        if (seatChoice < 1 || seatChoice > totalSeats) {
            cout << "Seat " << seatChoice << " is invalid, ignored.\n";
        } else if (find(bookedSeats.begin(), bookedSeats.end(), seatChoice) != bookedSeats.end()) {
            cout << "Seat " << seatChoice << " is already booked!\n";
        } else if (find(selectedSeats.begin(), selectedSeats.end(), seatChoice) != selectedSeats.end()) {
            cout << "Seat " << seatChoice << " is already selected, ignored.\n";
        } else {
            selectedSeats.push_back(seatChoice);
        }
    }

    if (selectedSeats.empty()) {
        cout << "No valid seats selected.\n";
        return;
    }

    bookedSeats.insert(bookedSeats.end(), selectedSeats.begin(), selectedSeats.end());
    saveBookedSeats(busId, bookedSeats);

    cout << "Seats booked successfully: ";
    for (int seat : selectedSeats) cout << seat << " ";
    cout << endl;

    passengerInfoAndPayment(selectedSeats, busId);
}

// ---------- Dashboard ----------
void passengerInfoAndPayment(const vector<int>& seats, int busId) {
    // Placeholder
}

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
        if (line.empty()) continue;

        stringstream ss(line);
        string idStr, dateStr, src, dest, seatsStr, priceStr;

        // Parse: ID,DATE,SOURCE,DESTINATION,SEATS,PRICE
        if (!getline(ss, idStr, ',')) continue;
        if (!getline(ss, dateStr, ',')) continue;
        if (!getline(ss, src, ',')) continue;
        if (!getline(ss, dest, ',')) continue;
        if (!getline(ss, seatsStr, ',')) continue;
        if (!getline(ss, priceStr, ',')) continue;

        // Skip header row if present
        int idVal;
        try { idVal = stoi(idStr); }
        catch (...) { continue; }

        // Filter by exact date + route
        if (toLower(src) == sourceLower &&
            toLower(dest) == destinationLower &&
            dateStr == date) {

            Bus bus;
            bus.id = idVal;
            bus.date = dateStr;
            bus.source = src;
            bus.destination = dest;
            try { bus.seats = stoi(seatsStr); } catch (...) { bus.seats = 0; }
            try { bus.price = stoi(priceStr); } catch (...) { bus.price = 0; }
            matchedBuses.push_back(bus);
        }
    }
    file.close();

    if (matchedBuses.empty()) {
        cout << "No buses available for this date on this route.\n";
        return;
    }

    cout << "\nAvailable buses from " << source << " to " << destination << " on " << date << ":\n";
    cout << "No.\tBusID\tSource\t\tDestination\tSeats\tPrice\n";
    for (size_t i = 0; i < matchedBuses.size(); i++) {
        cout << (i + 1) << "\t"
             << matchedBuses[i].id << "\t"
             << matchedBuses[i].source << "\t\t"
             << matchedBuses[i].destination << "\t\t"
             << matchedBuses[i].seats << "\t"
             << matchedBuses[i].price << "\n";
    }

    int choice;
    cout << "Enter option number to choose (1-" << matchedBuses.size() << "): ";
    cin >> choice;
    if (choice < 1 || choice > (int)matchedBuses.size()) {
        cout << "Invalid option selected.\n";
        return;
    }

    Bus chosenBus = matchedBuses[choice - 1];
    cout << "You selected Bus ID: " << chosenBus.id << "\n";
    selectSeat(chosenBus.id);
}

void bookings() {
    cout << "Bookings functionality to be implemented...\n";
}

void updateProfile() {
    cout << "Update profile functionality to be implemented...\n";
}

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
            case 1: searchBus(); break;
            case 2: bookings(); break;
            case 3: updateProfile(); break;
            default: cout << "Invalid choice. Please enter 1, 2, or 3.\n"; continue;
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