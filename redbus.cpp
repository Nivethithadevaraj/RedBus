#include <iostream>
#include <string>

using namespace std;

void loginOrRegister() {
   
}

void dashboard() {
    cout << "WDashboard!\n";
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

int main() {
    loginOrRegister();
    dashboard();

    if (getYesNo("Do you want to logout? (Y/N): ")) {
        cout << "Logout successfully.\n";
    }

    return 0;
}
