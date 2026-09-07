
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <limits>

using namespace std;

const string USERS_FILE = "users.txt";
const string TRIPS_FILE  = "trips.txt";

void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

int getIntInput(const string &prompt) {
    int val;
    while (true) {
        cout << prompt;
        if (cin >> val) { cin.ignore(numeric_limits<streamsize>::max(), '\n'); return val; }
        cout << "Invalid input. Please enter a number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

double getDoubleInput(const string &prompt) {
    double val;
    while (true) {
        cout << prompt;
        if (cin >> val) { cin.ignore(numeric_limits<streamsize>::max(), '\n'); return val; }
        cout << "Invalid input. Please enter a number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string currentTimestamp() {
    time_t now = time(nullptr);
    char buf[32];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return string(buf);
}

bool isNightTime() {
    time_t now = time(nullptr);
    tm *lt = localtime(&now);
    return (lt->tm_hour >= 22 || lt->tm_hour < 6);
}


class User {
public:
    string name, phone, password, pickup, destination;

    string toFileLine() const {
        return name + "|" + phone + "|" + password;
    }
    static User fromFileLine(const string &line) {
        User u;
        stringstream ss(line);
        getline(ss, u.name, '|');
        getline(ss, u.phone, '|');
        getline(ss, u.password, '|');
        return u;
    }
};


struct Trip {
    string phone;
    string pickup, destination, cabType;
    double distance = 0;
    double fare = 0;
    string status; 
    string timestamp;

    string toFileLine() const {
        stringstream ss;
        ss << phone << "|" << pickup << "|" << destination << "|" << cabType << "|"
           << distance << "|" << fare << "|" << status << "|" << timestamp;
        return ss.str();
    }
    static Trip fromFileLine(const string &line) {
        Trip t;
        stringstream ss(line);
        string tok;
        getline(ss, t.phone, '|');
        getline(ss, t.pickup, '|');
        getline(ss, t.destination, '|');
        getline(ss, t.cabType, '|');
        getline(ss, tok, '|'); t.distance = stod(tok);
        getline(ss, tok, '|'); t.fare = stod(tok);
        getline(ss, t.status, '|');
        getline(ss, t.timestamp, '|');
        return t;
    }
};


class CabBookingSystem {
    vector<User> users;
    vector<Trip> trips;
    User *loggedInUser = nullptr;

public:
    CabBookingSystem() { loadUsers(); loadTrips(); }

    void loadUsers() {
        users.clear();
        ifstream fin(USERS_FILE);
        string line;
        while (getline(fin, line)) if (!line.empty()) users.push_back(User::fromFileLine(line));
    }
    void saveUsers() {
        ofstream fout(USERS_FILE, ios::trunc);
        for (auto &u : users) fout << u.toFileLine() << "\n";
    }
    void loadTrips() {
        trips.clear();
        ifstream fin(TRIPS_FILE);
        string line;
        while (getline(fin, line)) if (!line.empty()) trips.push_back(Trip::fromFileLine(line));
    }
    void saveTrips() {
        ofstream fout(TRIPS_FILE, ios::trunc);
        for (auto &t : trips) fout << t.toFileLine() << "\n";
    }

    User* findUser(const string &phone) {
        for (auto &u : users) if (u.phone == phone) return &u;
        return nullptr;
    }

    void registerUser() {
        User u;
        cout << "\n--- Register ---\n";
        cout << "Name: "; getline(cin, u.name);
        cout << "Phone: "; getline(cin, u.phone);
        if (findUser(u.phone)) { cout << "A user with this phone already exists.\n"; return; }
        cout << "Password: "; getline(cin, u.password);
        users.push_back(u);
        saveUsers();
        cout << "Registration successful! You can now login.\n";
    }

    void loginUser() {
        string phone, password;
        cout << "\n--- Login ---\n";
        cout << "Phone: "; getline(cin, phone);
        cout << "Password: "; getline(cin, password);
        User *u = findUser(phone);
        if (u && u->password == password) {
            loggedInUser = u;
            cout << "Login successful. Welcome, " << u->name << "!\n";
        } else {
            cout << "Invalid phone or password.\n";
        }
    }

    void logoutUser() {
        if (loggedInUser) {
            cout << "Goodbye, " << loggedInUser->name << "!\n";
            loggedInUser = nullptr;
        } else {
            cout << "No user is currently logged in.\n";
        }
    }

    double calculateFare(const string &cabType, double distance) {
        double baseFare = 0, perKm = 0;
        if (cabType == "Mini")      { baseFare = 30; perKm = 8; }
        else if (cabType == "Sedan"){ baseFare = 50; perKm = 12; }
        else if (cabType == "SUV")  { baseFare = 80; perKm = 16; }
        double fare = baseFare + perKm * distance;
        if (isNightTime()) fare *= 1.25; 
        return fare;
    }

    Trip* getActiveTrip() {
        for (auto it = trips.rbegin(); it != trips.rend(); ++it)
            if (it->phone == loggedInUser->phone &&
                (it->status == "Booked" || it->status == "Ongoing"))
                return &(*it);
        return nullptr;
    }

    void bookCab() {
        if (!loggedInUser) { cout << "Please login first.\n"; return; }
        if (getActiveTrip()) { cout << "You already have an active booking. Cancel it before booking a new one.\n"; return; }

        Trip t;
        t.phone = loggedInUser->phone;
        cout << "\n--- Book a Cab ---\n";
        cout << "Pickup location: "; getline(cin, t.pickup);
        cout << "Destination: "; getline(cin, t.destination);
        cout << "Choose cab type (1-Mini, 2-Sedan, 3-SUV): ";
        int choice; cin >> choice; cin.ignore(numeric_limits<streamsize>::max(), '\n');
        t.cabType = (choice == 1) ? "Mini" : (choice == 2) ? "Sedan" : "SUV";
        t.distance = getDoubleInput("Enter estimated distance (km): ");
        t.fare = calculateFare(t.cabType, t.distance);
        t.status = "Booked";
        t.timestamp = currentTimestamp();
        trips.push_back(t);
        saveTrips();
        cout << fixed << setprecision(2);
        cout << "Cab booked successfully! Estimated fare: Rs " << t.fare << "\n";
    }

    void viewCurrentBooking() {
        if (!loggedInUser) { cout << "Please login first.\n"; return; }
        Trip *t = getActiveTrip();
        if (!t) { cout << "No active booking found.\n"; return; }
        cout << fixed << setprecision(2);
        cout << "\n--- Current Booking ---\n";
        cout << "Pickup: " << t->pickup << "\nDestination: " << t->destination
             << "\nCab Type: " << t->cabType << "\nDistance: " << t->distance << " km"
             << "\nFare: Rs " << t->fare << "\nStatus: " << t->status
             << "\nBooked At: " << t->timestamp << "\n";
    }

    void cancelBooking() {
        if (!loggedInUser) { cout << "Please login first.\n"; return; }
        Trip *t = getActiveTrip();
        if (!t) { cout << "No active booking to cancel.\n"; return; }
        t->status = "Cancelled";
        saveTrips();
        cout << "Booking cancelled successfully.\n";
    }

    void completeActiveTrip() {
        if (!loggedInUser) { cout << "Please login first.\n"; return; }
        Trip *t = getActiveTrip();
        if (!t) { cout << "No active booking to complete.\n"; return; }
        t->status = "Completed";
        saveTrips();
        cout << "Trip marked as completed. Thanks for riding with us!\n";
    }

    void viewTripHistory() {
        if (!loggedInUser) { cout << "Please login first.\n"; return; }
        cout << fixed << setprecision(2);
        cout << "\n--- Trip History ---\n";
        bool found = false;
        for (auto &t : trips) {
            if (t.phone == loggedInUser->phone) {
                found = true;
                cout << "[" << t.timestamp << "] " << t.pickup << " -> " << t.destination
                     << " | " << t.cabType << " | Rs " << t.fare << " | " << t.status << "\n";
            }
        }
        if (!found) cout << "No trips found yet.\n";
    }

    bool isLoggedIn() const { return loggedInUser != nullptr; }

    void mainMenu() {
        int choice;
        do {
            cout << "\n===== CAB BOOKING APP =====\n";
            cout << "Logged in as: " << (loggedInUser ? loggedInUser->name : "Guest") << "\n";
            cout << "1. Register\n2. Login\n3. Logout\n4. Book a Cab\n"
                 << "5. View Current Booking\n6. Cancel Booking\n7. Complete Trip (simulate)\n"
                 << "8. View Trip History\n0. Exit\n";
            choice = getIntInput("Enter choice: ");
            switch (choice) {
                case 1: registerUser(); break;
                case 2: loginUser(); break;
                case 3: logoutUser(); break;
                case 4: bookCab(); break;
                case 5: viewCurrentBooking(); break;
                case 6: cancelBooking(); break;
                case 7: completeActiveTrip(); break;
                case 8: viewTripHistory(); break;
                case 0: cout << "Thank you for using the Cab Booking App!\n"; break;
                default: cout << "Invalid choice.\n";
            }
        } while (choice != 0);
    }
};

int main() {
    CabBookingSystem system;
    system.mainMenu();
    return 0;
}
