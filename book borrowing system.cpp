#include <iostream>
#include <string>
#include <vector>
#include <iomanip> // Required for setprecision (to show 0.00)

using namespace std;

const int ALLOWED_DAYS = 7;
const double FINE_PER_DAY = 2.0;

// Struct to hold Book Data
struct Book {
    string name;
    int bDay, bMonth, bYear; // Borrow Date
    int rDay, rMonth, rYear; // Return Date
};

int monthDays[13] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };

// Check for leap year
bool isLeap(int y) {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

bool isValidDate(int d, int m, int y) {
    if (y < 1 || m < 1 || m > 12) return false;
    int daysInMonth = monthDays[m];
    if (m == 2 && isLeap(y)) daysInMonth = 29;
    
    return d >= 1 && d <= daysInMonth;
}

long long convertToDays(int d, int m, int y) {
    long long total = (long long)(y - 1) * 365; 
    
    for (int i = 1; i < m; i++) {
        total += monthDays[i];
        if (i == 2 && isLeap(y)) total++;
    }
    total += d;
    return total;
}

int main() {
    string customer;
    int n;

    cout << "===== Library Borrowing System =====\n";
    cout << "Enter customer name: ";
    getline(cin, customer);

   cout << "How many books to borrow? ";
while (!(cin >> n)) {
    cout << "Error! Please enter numbers only: ";
    cin.clear();
    cin.ignore(1000, '\n');
}
    cin.ignore();

    vector<Book> library(n); 

    for (int i = 0; i < n; i++) {
        cout << "\n--- Book #" << i + 1 << " ---\n";
        cout << "Book name: ";
        getline(cin, library[i].name);

        // Input Borrow Date
        while (true) {
            cout << "Borrow Date (DD MM YYYY): ";
            cin >> library[i].bDay >> library[i].bMonth >> library[i].bYear;
            
            if (isValidDate(library[i].bDay, library[i].bMonth, library[i].bYear)) {
                break; 
            }
            cout << "Invalid Date! Please enter again.\n";
        }

        // Input Return Date
        while (true) {
            cout << "Return Date (DD MM YYYY): ";
            cin >> library[i].rDay >> library[i].rMonth >> library[i].rYear;

            if (isValidDate(library[i].rDay, library[i].rMonth, library[i].rYear)) {
                break;
            }
            cout << "Invalid Date! Please enter again.\n";
        }
        cin.ignore();
    }

    // The output for BORROWING SUMMARY
    cout << "\n========================================\n";
    cout << "           BORROWING SUMMARY            \n";
    cout << "========================================\n";
    cout << "Customer: " << customer << "\n";

    cout << fixed << setprecision(2);

    double grandTotalFine = 0;

    for (int i = 0; i < n; i++) {
        long long bTotal = convertToDays(library[i].bDay, library[i].bMonth, library[i].bYear);
        long long rTotal = convertToDays(library[i].rDay, library[i].rMonth, library[i].rYear);

        long long duration = rTotal - bTotal;
        long long overdue = duration - ALLOWED_DAYS;

        cout << "\n[" << i + 1 << "] Title: " << library[i].name << "\n";
        cout << "    Borrowed: " << library[i].bDay << "/" << library[i].bMonth << "/" << library[i].bYear;
        cout << " -> Returned: " << library[i].rDay << "/" << library[i].rMonth << "/" << library[i].rYear << "\n";
        cout << "    Duration: " << duration << " days\n";

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