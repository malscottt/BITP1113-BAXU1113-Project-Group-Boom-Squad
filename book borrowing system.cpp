#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits> // Required for numeric_limits
#include <sstream>

using namespace std;

// Constants defining rules for borrowing
const int ALLOWED_DAYS = 7;      // Maximum days allowed before fine
const double FINE_PER_DAY = 2.0; // Fine amount per overdue day

// Structure to represent a single book transaction
struct Book {
    string name;                 // Title of the book
    int bDay = 0, bMonth = 0, bYear = 0;     // Date borrowed (default 0)
    int rDay = 0, rMonth = 0, rYear = 0;     // Date returned (default 0)
};

// Array storing days in each month (index 1 = Jan, 2 = Feb, etc.)
int monthDays[13] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };

// Function to check if a year is a leap year
bool isLeap(int y) {
    // Leap year if divisible by 4 AND not 100, OR divisible by 400
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

// Function to validate if a date is correct
bool isValidDate(int d, int m, int y) {
    if (y < 1 || m < 1 || m > 12) return false; // Check year and month range
    int daysInMonth = monthDays[m];
    // Adjust for leap year in February
    if (m == 2 && isLeap(y)) daysInMonth = 29;
    return d >= 1 && d <= daysInMonth;          // Check day range
}

// Function to convert a date into a total count of days from year 0
long long convertToDays(int d, int m, int y) {
    long long total = (long long)(y - 1) * 365; // Days from full previous years
    for (int i = 1; i < m; i++) {
        total += monthDays[i];                  // Add days from full previous months
        if (i == 2 && isLeap(y)) total++;       // Add leap day if applicable
    }
    total += d;                                 // Add days from current month
    return total;
}

// Main Function
int main() {
    string customer;
    int n;

    // --- Input Customer Details ---
    cout << "===== Library Borrowing System v2.0 =====\n";
    cout << "Enter customer name: ";
    getline(cin, customer);

    // --- Input Number of Books ---
    cout << "How many books to borrow? ";
    // Loop to ensure valid integer input
    while (!(cin >> n)) {
        cout << "Error! Please enter numbers only: ";
        cin.clear();              // Clear error flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n');   // Discard invalid input
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear newline character left in buffer

    vector<Book> library(n); // Vector to store details of 'n' books

    for (int i = 0; i < n; i++) {
        cout << "\n--- Book #" << i + 1 << " ---\n";
        cout << "Book name: ";
        getline(cin, library[i].name);

        // --- Input Borrow Date with Validation ---
        while (true) {
            cout << "Borrow Date (DD MM YYYY): ";
            string line;
            if (!getline(cin, line)) { // Check for EOF
                 return 0;
            }
            stringstream ss(line);
            if (ss >> library[i].bDay >> library[i].bMonth >> library[i].bYear) {
                 // Check valid date logic
                if (isValidDate(library[i].bDay, library[i].bMonth, library[i].bYear)) {
                    break; // Valid date
                }
                cout << "Invalid Date! Please enter again.\n";
            } else {
                cout << "Invalid input! Please enter numbers only.\n";
            }
        }

        // --- Input Return Date with Validation ---
        while (true) {
            cout << "Return Date (DD MM YYYY): ";
            string line;
            if (!getline(cin, line)) {
                return 0;
            }
            stringstream ss(line);
            if (ss >> library[i].rDay >> library[i].rMonth >> library[i].rYear) {
                 if (isValidDate(library[i].rDay, library[i].rMonth, library[i].rYear)) {
                    // Calculate total days for comparison
                    long long borrowDays = convertToDays(
                        library[i].bDay, library[i].bMonth, library[i].bYear);
                    long long returnDays = convertToDays(
                        library[i].rDay, library[i].rMonth, library[i].rYear);

                    // Ensure return date is not before borrow date
                    if (returnDays < borrowDays) {
                        cout << "Error! Return date cannot be before borrow date.\n";
                        continue;
                    }
                    break; // Valid return date
                 }
                 cout << "Invalid Date! Please enter again.\n";
            } else {
                 cout << "Invalid input! Please enter numbers only.\n";
            }
        }
    }

    cout << "\n========================================\n";
    cout << "           BORROWING SUMMARY            \n";
    cout << "========================================\n";
    cout << "Customer: " << customer << "\n";

    cout << fixed << setprecision(2); // Set format for currency output
    double grandTotalFine = 0;

    // --- Loop to Calculate Fines and Display Status ---
    for (int i = 0; i < n; i++) {
        // Convert both dates to total days count from year 0
        long long bTotal = convertToDays(library[i].bDay, library[i].bMonth, library[i].bYear);
        long long rTotal = convertToDays(library[i].rDay, library[i].rMonth, library[i].rYear);

        long long duration = rTotal - bTotal;        // Calculate actual borrowing duration
        long long overdue = duration - ALLOWED_DAYS; // Calculate overdue days (if any)

        cout << "\n[" << i + 1 << "] Title: " << library[i].name << "\n";
        cout << "    Borrowed: " << library[i].bDay << "/" << library[i].bMonth << "/" << library[i].bYear;
        cout << " -> Returned: " << library[i].rDay << "/" << library[i].rMonth << "/" << library[i].rYear << "\n";
        cout << "    Duration: " << duration << " days\n";

        // Check availability of fine
        if (overdue > 0) {
            double fine = overdue * FINE_PER_DAY;
            grandTotalFine += fine;
            cout << "    STATUS: [OVERDUE] by " << overdue << " days.\n";
            cout << "    FINE: RM " << fine << "\n";
        } else {
            cout << "    STATUS: [ON TIME]\n";
        }
    }

    cout << "\n----------------------------------------\n";
    cout << "Total Fine to Pay: RM " << grandTotalFine << "\n";
    cout << "----------------------------------------\n";
    cout << "\nTHANK YOU\n";

    return 0;
}