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

struct Passenger {
    string name;
    int age;
    char gender;
    string phone;
    int seatNumber;
    int busId;
};

void passengerInfoAndPayment(const vector<int>& seats, int busId);
void payment(const vector<int>& seats, int busId);

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

        srand(time(0));
        int otp = rand() % 9000 + 1000;
        cout << "Your OTP is: " << otp << endl;

        if (verifyOTP(otp)) {
            if ((choice == 'R' || choice == 'r') && !isPhoneNumberExists(phoneNumber)) {
                saveUserToCSV(phoneNumber);
            }
            return true;
        } else {
            if (!getYesNo("Try again? (Y/N): ")) return false;
        }
    }
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
    cin.ignore();
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
    vector<Passenger> passengers;
    for (int seat : seats) {
        Passenger p;
        p.busId = busId;
        p.seatNumber = seat;
        cout << "\n-- Seat " << seat << " --\n";

        do {
            cout << "Phone (10 digits): ";
            cin >> p.phone;
        } while (!isValidPhoneNumber(p.phone));

        cout << "Name: ";
        cin.ignore();
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

        passengers.push_back(p);
    }

    ofstream out("Passengers.csv", ios::app);
    for (const Passenger& p : passengers) {
        out << p.busId << "," << p.seatNumber << "," << p.name << ","
            << p.age << "," << p.gender << "," << p.phone << endl;
    }
    out.close();

    payment(seats, busId);
}

void payment(const vector<int>& seats, int busId) {
    ifstream file("Buses.csv");
    string line;
    int pricePerSeat = 0;

    while (getline(file, line)) {
        stringstream ss(line);
        string idStr, date, src, dest, seatsStr, priceStr;
        getline(ss, idStr, ',');
        try {
            int id = stoi(idStr);
            if (id == busId) {
                getline(ss, date, ','); getline(ss, src, ','); getline(ss, dest, ',');
                getline(ss, seatsStr, ','); getline(ss, priceStr, ',');
                pricePerSeat = stoi(priceStr);
                break;
            }
        } catch (...) {}
    }

    int totalAmount = pricePerSeat * seats.size();
    cout << "Total: ₹" << totalAmount << "\nChoose payment method (1. UPI 2. Card 3. NetBanking): ";
    int method;
    cin >> method;

    srand(time(0));  // Seed once here

    if (method == 1) { // UPI
        string upiId, pin;
        cout << "UPI Payment Selected.\n";
        cout << "Enter UPI ID: ";
        cin >> upiId;
        cout << "Enter PIN: ";
        cin >> pin;
        cout << "Authenticating...\n";
    } else if (method == 2) { // Card
        int otp = rand() % 9000 + 1000;
        cout << "Card Payment Selected.\nYour OTP is: " << otp << endl;
        if (!verifyOTP(otp)) {
            cout << "OTP Verification Failed. Booking Cancelled.\n";
            return;
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
            return;
        }
    } else {
        cout << "Invalid payment method.\n";
        return;
    }

    cout << "Processing payment...\n";
    bool success = rand() % 2;

    if (success) {
        cout << "Payment Successful!\n";
        vector<int> finalSeats = loadBookedSeats(busId);
        finalSeats.insert(finalSeats.end(), seats.begin(), seats.end());
        saveBookedSeats(busId, finalSeats);
        cout << "Booking Confirmed. Thank you!\n";
    } else {
        cout << "Payment Failed. Booking Cancelled.\n";

        // Remove passenger info from file
        ifstream in("Passengers.csv");
        vector<string> lines;
        while (getline(in, line)) {
            bool toDelete = false;
            for (int s : seats)
                if (line.find(to_string(busId) + "," + to_string(s) + ",") == 0)
                    toDelete = true;
            if (!toDelete) lines.push_back(line);
        }
        in.close();
        ofstream out("Passengers.csv");
        for (string& l : lines) out << l << endl;
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
void bookings(){

}
void updateProfile(){
    
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
    if (processLoginOrRegister()) {
        // dashboard();
        if (getYesNo("Logout? (Y/N): ")) cout << "Logged out.\n";
    } else {
        cout << "Login/Register failed.\n";
    }
    return 0;
}
