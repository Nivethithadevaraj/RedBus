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

struct Bus {
    int id;
    string date;
    string source;
    string destination;
    int seats;
    int price;
};

string loggedInPhone;

struct Passenger {
    string name;
    int age;
    char gender;
    string phone;
    int seatNumber;
    int busId;
    string ticketId;
};

// forward declarations
string generateTicketID();
bool ticketIdExists(const string& id);
void passengerInfoAndPayment(const vector<int>& seats, int busId);
bool payment(const vector<int>& seats, int busId, int totalAmount);
bool getYesNo(const string& prompt);
string toLower(const string& s);
bool isValidPhoneNumber(const string& phone);
bool isPhoneNumberExists(const string& phoneNumber);
void saveUserToCSV(const string& phoneNumber);
bool verifyOTP(int generatedOTP);
bool processLoginOrRegister();
bool isLeapYear(int year);
bool isValidDate(const string& dateStr);
void inputs(string& source, string& destination, string& date);
vector<int> loadBookedSeats(int busId);
void saveBookedSeats(int busId, const vector<int>& bookedSeats);
void selectSeat(int busId);
void searchBus();
string getBusDate(int busId);
void cancelTicket(const string& ticketId);
void currentBookings();
void bookings();
void updateProfile();
void dashboard();


// generate a 3-digit ticket id (100..999) — no reseeding here
string generateTicketID() {
    string id;
    do {
        int num = rand() % 900 + 100; // 100..999
        id = to_string(num);
    } while (ticketIdExists(id));
    return id;
}

bool ticketIdExists(const string& id) {
    ifstream in("Passengers.csv");
    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string tId;
        getline(ss, tId, ',');
        if (tId == id) return true;
    }
    return false;
}

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
    return phone.length() == 10 && all_of(phone.begin(), phone.end(), ::isdigit);
}

bool isPhoneNumberExists(const string& phoneNumber) {
    ifstream file("Users.csv");
    string line;
    while (getline(file, line)) {
        if (line == phoneNumber) return true;
    }
    return false;
}

void saveUserToCSV(const string& phoneNumber) {
    ofstream file("Users.csv", ios::app);
    file << phoneNumber << endl;
    cout << "Phone number saved to Users.csv.\n";
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
            cout << "Invalid phone number. Try again.\n";
            continue;
        }

        if ((choice == 'R' || choice == 'r') && isPhoneNumberExists(phoneNumber)) {
            cout << "Phone number already exists.\n";
            continue;
        }

        int otp = rand() % 9000 + 1000;
        cout << "Your OTP is: " << otp << endl;

        if (verifyOTP(otp)) {
            if ((choice == 'R' || choice == 'r') && !isPhoneNumberExists(phoneNumber)) {
                saveUserToCSV(phoneNumber);
            }
            loggedInPhone = phoneNumber;  // store the logged-in number
            return true;
        } else {
            if (!getYesNo("Try again? (Y/N): ")) return false;
        }
    }
}
bool isValidDOB(const string& dobStr) {
    if (dobStr.length() != 10 || dobStr[2] != '-' || dobStr[5] != '-') return false;
    int day, month, year;
    try {
        day = stoi(dobStr.substr(0, 2));
        month = stoi(dobStr.substr(3, 2));
        year = stoi(dobStr.substr(6, 4));
    } catch (...) {
        return false;
    }

    if (year < 1900 || year > 2100 || month < 1 || month > 12) return false;
    int daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (month == 2 && isLeapYear(year)) daysInMonth[1] = 29;
    if (day < 1 || day > daysInMonth[month - 1]) return false;

    // Compare with current date (DOB must be before today)
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    int currYear = now->tm_year + 1900;
    int currMonth = now->tm_mon + 1;
    int currDay = now->tm_mday;

    if (year > currYear) return false;
    if (year == currYear && month > currMonth) return false;
    if (year == currYear && month == currMonth && day >= currDay) return false;

    return true;
}

bool isLeapYear(int year) {
    return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
}

bool isValidDate(const string& dateStr) {
    if (dateStr.length() != 10 || dateStr[2] != '-' || dateStr[5] != '-') return false;
    int day, month, year;
    try {
        day = stoi(dateStr.substr(0, 2));
        month = stoi(dateStr.substr(3, 2));
        year = stoi(dateStr.substr(6, 4));
    } catch (...) {
        return false;
    }
    if (year < 1900 || year > 2100 || month < 1 || month > 12) return false;
    int daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (month == 2 && isLeapYear(year)) daysInMonth[1] = 29;
    if (day < 1 || day > daysInMonth[month - 1]) return false;

    time_t t = time(nullptr);
    tm* now = localtime(&t);
    int currYear = now->tm_year + 1900;
    int currMonth = now->tm_mon + 1;
    int currDay = now->tm_mday;

    if (year < currYear) return false;
    if (year == currYear && month < currMonth) return false;
    if (year == currYear && month == currMonth && day < currDay) return false;
    return true;
}

void inputs(string& source, string& destination, string& date) {
    cout << "Enter Source: ";
    cin >> source;
    cout << "Enter Destination: ";
    cin >> destination;
    while (true) {
        cout << "Enter Date (DD-MM-YYYY): ";
        cin >> date;
        if (isValidDate(date)) break;
        else cout << "Invalid date. Try again.\n";
    }
}

vector<int> loadBookedSeats(int busId) {
    vector<int> bookedSeats;
    ifstream file("Seats.csv");
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string idStr;
        getline(ss, idStr, ',');
        try {
            int id = stoi(idStr);
            if (id == busId) {
                string seatStr;
                while (getline(ss, seatStr, ',')) {
                    if (!seatStr.empty())
                        bookedSeats.push_back(stoi(seatStr));
                }
                break;
            }
        } catch (...) {}
    }
    return bookedSeats;
}

void saveBookedSeats(int busId, const vector<int>& bookedSeats) {
    ifstream in("Seats.csv");
    vector<string> lines;
    bool found = false;
    string line;

    while (getline(in, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string idStr;
        getline(ss, idStr, ',');
        int id = stoi(idStr);
        if (id == busId) {
            string newLine = to_string(busId);
            for (int seat : bookedSeats) newLine += "," + to_string(seat);
            lines.push_back(newLine);
            found = true;
        } else lines.push_back(line);
    }
    in.close();

    if (!found) {
        string newLine = to_string(busId);
        for (int seat : bookedSeats) newLine += "," + to_string(seat);
        lines.push_back(newLine);
    }

    ofstream out("Seats.csv");
    for (auto& l : lines) out << l << endl;
}

void selectSeat(int busId) {
    vector<int> bookedSeats = loadBookedSeats(busId);
    const int totalSeats = 20;
    vector<int> availableSeats;

    for (int i = 1; i <= totalSeats; ++i)
        if (find(bookedSeats.begin(), bookedSeats.end(), i) == bookedSeats.end())
            availableSeats.push_back(i);

    if (availableSeats.empty()) {
        cout << "All seats are booked.\n";
        return;
    }

    cout << "Available seats: ";
    for (int s : availableSeats) cout << s << " ";
    cout << "\nEnter seat numbers (space-separated): ";

    vector<int> selectedSeats;
    string line;
    int seat;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, line);
    stringstream ss(line);

    while (ss >> seat) {
        if (seat < 1 || seat > totalSeats) cout << "Invalid seat: " << seat << endl;
        else if (find(bookedSeats.begin(), bookedSeats.end(), seat) != bookedSeats.end())
            cout << "Seat " << seat << " already booked!\n";
        else selectedSeats.push_back(seat);
    }

    if (selectedSeats.empty()) {
        cout << "No valid seats selected.\n";
        return;
    }

    passengerInfoAndPayment(selectedSeats, busId);
}

void passengerInfoAndPayment(const vector<int>& seats, int busId) {
    // Collect passenger info locally first (don't write to file yet)
    vector<Passenger> passengers;
    string date = getBusDate(busId);
    if (date.empty()) date = "00-00-0000";

    for (int seat : seats) {
        Passenger p;
        p.busId = busId;
        p.seatNumber = seat;
        p.phone = loggedInPhone; // do not ask phone again; logged-in user used for bookings
        cout << "\n-- Seat " << seat << " --\n";

        cout << "Name: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, p.name);

        do {
            cout << "Age: ";
            cin >> p.age;
        } while (p.age <= 0 || p.age > 120);

        do {
            cout << "Gender (M/F/O): ";
            cin >> p.gender;
            p.gender = toupper(p.gender);
        } while (p.gender != 'M' && p.gender != 'F' && p.gender != 'O');

        p.ticketId = generateTicketID();
        passengers.push_back(p);
    }

    // Determine price per seat
    ifstream file("Buses.csv");
    string line;
    int pricePerSeat = 0;
    while (getline(file, line)) {
        stringstream ss(line);
        string idStr, dateStr, src, dest, seatStr, priceStr;
        getline(ss, idStr, ',');
        try {
            int id = stoi(idStr);
            if (id == busId) {
                getline(ss, dateStr, ','); getline(ss, src, ','); getline(ss, dest, ',');
                getline(ss, seatStr, ','); getline(ss, priceStr, ',');
                pricePerSeat = stoi(priceStr);
                break;
            }
        } catch (...) {}
    }
    file.close();

    int totalAmount = pricePerSeat * seats.size();
    cout << "Total amount to pay: ₹" << totalAmount << endl;

    // Run payment; on success, append all passenger entries and update seats
    if (!payment(seats, busId, totalAmount)) {
        cout << "Payment failed. Booking cancelled.\n";
        return;
    }

    // Append passengers to file now
    ofstream out("Passengers.csv", ios::app);
    for (const Passenger& p : passengers) {
        out << p.ticketId << "," << p.busId << "," << p.seatNumber << "," 
            << date << "," << p.name << "," << p.age << "," << p.gender << "," << p.phone << endl;
        cout << "Booking Confirmed! Ticket ID: " << p.ticketId << " | Seat: " << p.seatNumber << "\n";
    }
    out.close();

    // mark seats as booked now
    vector<int> finalSeats = loadBookedSeats(busId);
    finalSeats.insert(finalSeats.end(), seats.begin(), seats.end());
    // remove possible duplicates and sort
    sort(finalSeats.begin(), finalSeats.end());
    finalSeats.erase(unique(finalSeats.begin(), finalSeats.end()), finalSeats.end());
    saveBookedSeats(busId, finalSeats);
}

bool payment(const vector<int>& seats, int busId, int totalAmount) {
    cout << "Choose payment method (1. UPI 2. Card 3. NetBanking): ";
    int method;
    cin >> method;

    if (method == 1) { // UPI
        string upiId, pin;
        cout << "UPI Payment Selected.\n";
        cout << "Enter UPI ID: ";
        cin >> upiId;
        cout << "Enter PIN: ";
        cin >> pin;
        cout << "Authenticating...\n";
        // Simulate success/failure
    } else if (method == 2) { // Card
        int otp = rand() % 9000 + 1000;
        cout << "Card Payment Selected.\nYour OTP is: " << otp << endl;
        if (!verifyOTP(otp)) {
            cout << "OTP Verification Failed. Booking Cancelled.\n";
            return false;
        }
    } else if (method == 3) { // NetBanking
        string accNo, cvv;
        cout << "NetBanking Selected.\n";
        cout << "Enter Account Number: ";
        cin >> accNo;
        cout << "Enter CVV: ";
        cin >> cvv;
        int otp = rand() % 9000 + 1000;
        cout << "An OTP has been sent: " << otp << endl;
        if (!verifyOTP(otp)) {
            cout << "OTP Verification Failed. Booking Cancelled.\n";
            return false;
        }
    } else {
        cout << "Invalid payment method.\n";
        return false;
    }

    cout << "Processing payment...\n";
    bool success = (rand() % 2 == 0); // 50% chance success (simulation)
    if (success) {
        cout << "Payment Successful!\n";
        return true;
    } else {
        cout << "Payment Failed.\n";
        return false;
    }
}

void searchBus() {
    string source, destination, date;
    inputs(source, destination, date);
    string s = toLower(source), d = toLower(destination);

    ifstream file("Buses.csv");
    string line;
    vector<Bus> buses;

    while (getline(file, line)) {
        stringstream ss(line);
        string idStr, dateStr, src, dest, seatStr, priceStr;
        getline(ss, idStr, ',');
        try {
            int id = stoi(idStr);
            getline(ss, dateStr, ','); getline(ss, src, ','); getline(ss, dest, ',');
            getline(ss, seatStr, ','); getline(ss, priceStr, ',');

            if (toLower(src) == s && toLower(dest) == d && dateStr == date) {
                Bus b = {id, dateStr, src, dest, stoi(seatStr), stoi(priceStr)};
                buses.push_back(b);
            }
        } catch (...) {}
    }

    if (buses.empty()) {
        cout << "No buses found.\n";
        return;
    }

    cout << "Buses:\nNo. ID  Src -> Dest  Seats  Price\n";
    for (size_t i = 0; i < buses.size(); ++i)
        cout << i + 1 << ". " << buses[i].id << " " << buses[i].source << " -> "
             << buses[i].destination << " " << buses[i].seats << " ₹" << buses[i].price << "\n";

    int choice;
    cout << "Select bus (1-" << buses.size() << "): ";
    cin >> choice;
    if (choice < 1 || choice > (int)buses.size()) {
        cout << "Invalid choice.\n";
        return;
    }

    selectSeat(buses[choice - 1].id);
}

string getBusDate(int busId) {
    ifstream file("Buses.csv");
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string idStr, date, src, dest, seatStr, priceStr;
        getline(ss, idStr, ',');
        try {
            int id = stoi(idStr);
            if (id == busId) {
                getline(ss, date, ',');
                return date;
            }
        } catch (...) {}
    }
    return "";
}

void cancelTicket(const string& ticketId) {
    if (!getYesNo("Do you want to cancel this ticket? (Y/N): ")) return;

    ifstream in("Passengers.csv");
    vector<string> lines;
    string line;
    bool found = false;
    int busId = -1, seatNum = -1;

    while (getline(in, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string tId, busIdStr, seatStr, date, name, age, gender, phoneInFile;
        getline(ss, tId, ',');
        getline(ss, busIdStr, ',');
        getline(ss, seatStr, ',');
        getline(ss, date, ',');
        getline(ss, name, ',');
        getline(ss, age, ',');
        getline(ss, gender, ',');
        getline(ss, phoneInFile);

        if (tId == ticketId && phoneInFile == loggedInPhone) {
            found = true;
            try {
                busId = stoi(busIdStr);
                seatNum = stoi(seatStr);
            } catch (...) { }
            continue; // skip this line (delete)
        }
        lines.push_back(line);
    }
    in.close();

    if (!found) {
        cout << "Ticket not found or phone mismatch.\n";
        return;
    }

    ofstream out("Passengers.csv");
    for (auto& l : lines) out << l << endl;
    out.close();

    if (busId != -1 && seatNum != -1) {
        vector<int> seats = loadBookedSeats(busId);
        seats.erase(remove(seats.begin(), seats.end(), seatNum), seats.end());
        saveBookedSeats(busId, seats);
    }

    cout << "Ticket cancelled successfully. Amount will be refunded shortly.\n";
}

void currentBookings() {
    ifstream in("Passengers.csv");
    vector<string> lines;
    string line;

    cout << "\n--- CURRENT BOOKINGS ---\n";

    while (getline(in, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string ticketId, busIdStr, seatStr, date, name, ageStr, gender, phoneInFile;
        getline(ss, ticketId, ',');
        getline(ss, busIdStr, ',');
        getline(ss, seatStr, ',');
        getline(ss, date, ',');
        getline(ss, name, ',');
        getline(ss, ageStr, ',');
        getline(ss, gender, ',');
        getline(ss, phoneInFile);

        if (phoneInFile == loggedInPhone && isValidDate(date)) {
            cout << "Ticket ID: " << ticketId << ", Name: " << name << ", Date: " << date << ", Seat: " << seatStr << "\n";
            lines.push_back(line);
        }
    }
    in.close();

    if (lines.empty()) {
        cout << "No current bookings found.\n";
        return;
    }

    while (true) {
        string selectedId;
        cout << "\nEnter Ticket ID to view details: ";
        cin >> selectedId;

        bool found = false;
        for (const string& ticketLine : lines) {
            stringstream ss(ticketLine);
            string ticketId, busIdStr, seatStr, date, name, ageStr, gender, phoneInFile;
            getline(ss, ticketId, ',');
            getline(ss, busIdStr, ',');
            getline(ss, seatStr, ',');
            getline(ss, date, ',');
            getline(ss, name, ',');
            getline(ss, ageStr, ',');
            getline(ss, gender, ',');
            getline(ss, phoneInFile);

            if (ticketId == selectedId) {
                found = true;
                cout << "\n--- TICKET DETAILS ---\n";
                cout << "Ticket ID: " << ticketId << "\n";
                cout << "Bus ID: " << busIdStr << "\n";
                cout << "Seat: " << seatStr << "\n";
                cout << "Name: " << name << "\n";
                cout << "Age: " << ageStr << "\n";
                cout << "Gender: " << gender << "\n";
                cout << "Phone: " << phoneInFile << "\n";
                cout << "Date: " << date << "\n";

                if (getYesNo("Do you want to cancel this ticket? (Y/N): ")) {
                    cancelTicket(ticketId);
                }
                break;
            }
        }

        if (!found) cout << "Ticket ID not found.\n";

        if (!getYesNo("Do you want to view another ticket? (Y/N): ")) break;
    }
}

void bookings() {
    while (true) {
        cout << "\n--- BOOKINGS ---\n";
        cout << "1. Past Bookings\n";
        cout << "2. Current Bookings\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            cout << "\n--- PAST BOOKINGS ---\n";
            ifstream in("Passengers.csv");
            string line;
            bool found = false;
            while (getline(in, line)) {
                if (line.empty()) continue;
                stringstream ss(line);
                string ticketId, busIdStr, seatStr, date, name, ageStr, gender, phoneInFile;
                getline(ss, ticketId, ',');
                getline(ss, busIdStr, ',');
                getline(ss, seatStr, ',');
                getline(ss, date, ',');
                getline(ss, name, ',');
                getline(ss, ageStr, ',');
                getline(ss, gender, ',');
                getline(ss, phoneInFile);

                if (phoneInFile == loggedInPhone) {
                    if (!isValidDate(date)) { // past date
                        found = true;
                        cout << "Ticket ID: " << ticketId << ", Bus ID: " << busIdStr << ", Seat: " << seatStr << ", Name: " << name << ", Date: " << date << "\n";
                    }
                }
            }
            if (!found) cout << "No past bookings found.\n";
        } else if (choice == 2) {
            currentBookings();
        } else {
            cout << "Invalid choice.\n";
        }

        if (!getYesNo("Do you want to check bookings again? (Y/N): ")) break;
    }
}

void updateProfile() {
    string name, dob, gender, phone = loggedInPhone, email = "";

    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

    cout << "\n--- UPDATE PROFILE ---\n";

    cout << "Full Name: ";
    getline(cin, name);

    while (true) {
        cout << "Date of Birth (DD-MM-YYYY): ";
        getline(cin, dob);
        if (isValidDOB(dob)) break;
        else cout << "Invalid date. Try again.\n";
    }

    while (true) {
        cout << "Gender (M/F/O): ";
        getline(cin, gender);
        if (gender.length() == 1 && (gender[0] == 'M' || gender[0] == 'F' || gender[0] == 'O' ||
                                     gender[0] == 'm' || gender[0] == 'f' || gender[0] == 'o')) {
            gender[0] = toupper(gender[0]);
            break;
        } else {
            cout << "Invalid input. Please enter M, F, or O.\n";
        }
    }

    cout << "Registered Phone Number: " << phone << endl;

    if (getYesNo("Do you want to update email? (Y/N): ")) {
        cout << "Email: ";
        getline(cin, email);
    }

    // Read existing profiles and check if user already exists
    ifstream inFile("Profiles.csv");
    vector<string> lines;
    string line;
    bool found = false;

    while (getline(inFile, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string existingPhone;
        getline(ss, existingPhone, ',');

        if (existingPhone == phone) {
            // Replace with updated info
            string newLine = phone + "," + name + "," + dob + "," + gender + "," + email;
            lines.push_back(newLine);
            found = true;
        } else {
            lines.push_back(line);
        }
    }
    inFile.close();

    if (!found) {
        string newLine = phone + "," + name + "," + dob + "," + gender + "," + email;
        lines.push_back(newLine);
    }

    ofstream outFile("Profiles.csv");
    for (const string& l : lines) {
        outFile << l << endl;
    }
    outFile.close();

    cout << "Profile updated successfully!\n";
}


void dashboard() {
    while (true) {
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
            default: cout << "Invalid choice.\n"; break;
        }
    }
}

int main() {
    srand((unsigned)time(0)); // seed RNG once
    cout << "Welcome to Bus Booking System\n";

    if (processLoginOrRegister()) {
        if (getYesNo("Do you want to go to the dashboard? (Y/N): ")) {
            dashboard();
            if (getYesNo("Logout? (Y/N): ")) {
                cout << "Logged out.\n";
            }
        } else {
            cout << "You chose not to enter the dashboard. Exiting.\n";
        }
    } else {
        cout << "Login/Register failed.\n";
    }

    return 0;
}
