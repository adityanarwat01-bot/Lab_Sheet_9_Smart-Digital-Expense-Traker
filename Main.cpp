#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <limits>
using namespace std;

// Structure to store expense details
struct Expense {
    int id;
    string date;
    string name;
    double amount;
    string category;
};

// Global expense list
vector<Expense> expenses;
double monthlyBudget = 0;

// Read a complete line
string getText(string message) {
    string value;
    cout << message;
    getline(cin, value);
    return value;
}

// Register a new user
void registerUser() {
    string username, password;

    username = getText("Enter username: ");
    password = getText("Enter password: ");

    ifstream check("users.txt");
    string u, p;
    while (check >> u >> p) {
        if (u == username) {
            cout << "Username already exists!\n";
            return;
        }
    }
    check.close();

    ofstream file("users.txt", ios::app);
    file << username << " " << password << endl;
    file.close();

    cout << "Registration successful!\n";
}

// Login system
bool loginUser() {
    string username, password, u, p;

    username = getText("Enter username: ");
    password = getText("Enter password: ");

    ifstream file("users.txt");

    while (file >> u >> p) {
        if (u == username && p == password) {
            cout << "Login successful!\n";
            return true;
        }
    }

    cout << "Invalid username or password!\n";
    return false;
}

// Load expenses from file
void loadExpenses() {
    expenses.clear();
    ifstream file("expenses.txt");
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        Expense e;
        string amount;

        if (getline(ss, line, '|')) e.id = stoi(line);
        if (getline(ss, e.date, '|') &&
            getline(ss, e.name, '|') &&
            getline(ss, amount, '|') &&
            getline(ss, e.category, '|')) {
            e.amount = stod(amount);
            expenses.push_back(e);
        }
    }
    file.close();
}

// Save expenses to file
void saveExpenses() {
    ofstream file("expenses.txt");

    for (const auto &e : expenses) {
        file << e.id << "|"
             << e.date << "|"
             << e.name << "|"
             << e.amount << "|"
             << e.category << endl;
    }

    file.close();
}

// Add daily expense
void addExpense() {
    Expense e;

    int nextId = 1;
    for (const auto &x : expenses)
        nextId = max(nextId, x.id + 1);

    e.id = nextId;
    e.date = getText("Enter date (YYYY-MM-DD): ");
    e.name = getText("Enter expense name: ");
    e.category = getText("Enter category (Food/Travel/Shopping): ");

    cout << "Enter amount: ";
    if (!(cin >> e.amount) || e.amount <= 0) {
        cout << "Invalid amount!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    expenses.push_back(e);
    saveExpenses();

    cout << "Expense added successfully!\n";
}

// View expense history
void viewHistory() {
    if (expenses.empty()) {
        cout << "No expenses found.\n";
        return;
    }

    sort(expenses.begin(), expenses.end(),
         [](Expense a, Expense b) {
             return a.date < b.date;
         });

    cout << "\nID\tDate\t\tName\t\tAmount\tCategory\n";

    for (const auto &e : expenses) {
        cout << e.id << "\t"
             << e.date << "\t"
             << e.name << "\t\t"
             << e.amount << "\t"
             << e.category << endl;
    }
}

// Category-wise expense
void categoryExpense() {
    string category = getText("Enter category: ");
    double total = 0;

    for (const auto &e : expenses) {
        if (e.category == category)
            total += e.amount;
    }

    cout << "Total spending in " << category
         << " = Rs. " << total << endl;
}

// Monthly expense report
void monthlyReport() {
    string month = getText("Enter month (YYYY-MM): ");
    double total = 0;

    for (const auto &e : expenses) {
        if (e.date.substr(0, 7) == month)
            total += e.amount;
    }

    cout << "Monthly expense = Rs. " << total << endl;

    if (monthlyBudget > 0) {
        cout << "Monthly budget = Rs. "
             << monthlyBudget << endl;
        cout << "Remaining budget = Rs. "
             << monthlyBudget - total << endl;
    }
}

// Set monthly budget
void setBudget() {
    cout << "Enter monthly budget: ";

    if (!(cin >> monthlyBudget) || monthlyBudget <= 0) {
        cout << "Invalid budget!\n";
        monthlyBudget = 0;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    ofstream file("budget.txt");
    file << monthlyBudget;
    file.close();

    cout << "Budget saved successfully!\n";
}

// Load saved budget
void loadBudget() {
    ifstream file("budget.txt");
    if (file)
        file >> monthlyBudget;
    file.close();
}

// Budget warning
void budgetWarning() {
    if (monthlyBudget <= 0) {
        cout << "Please set your monthly budget first.\n";
        return;
    }

    double total = 0;
    string month = getText("Enter month (YYYY-MM): ");

    for (const auto &e : expenses) {
        if (e.date.substr(0, 7) == month)
            total += e.amount;
    }

    cout << "Total spending: Rs. " << total << endl;

    if (total >= monthlyBudget)
        cout << "WARNING: Monthly budget exceeded!\n";
    else if (total >= monthlyBudget * 0.8)
        cout << "WARNING: You are close to your budget!\n";
    else
        cout << "Your spending is within budget.\n";
}

// Predict future expenses
void expensePrediction() {
    string month = getText("Enter month (YYYY-MM): ");
    int daysElapsed, totalDays;

    cout << "Enter days elapsed in this month: ";
    cin >> daysElapsed;

    cout << "Enter total days in this month: ";
    cin >> totalDays;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (daysElapsed <= 0 || totalDays < daysElapsed) {
        cout << "Invalid number of days!\n";
        return;
    }

    double total = 0;

    for (const auto &e : expenses) {
        if (e.date.substr(0, 7) == month)
            total += e.amount;
    }

    double predicted = (total / daysElapsed) * totalDays;

    cout << "Current expense: Rs. " << total << endl;
    cout << "Predicted monthly expense: Rs. "
         << predicted << endl;

    if (monthlyBudget > 0 && predicted > monthlyBudget)
        cout << "Prediction: You may exceed your budget!\n";
    else
        cout << "Prediction: Spending is within budget.\n";
}

// Search expense
void searchExpense() {
    string keyword = getText("Enter expense name to search: ");
    bool found = false;

    for (const auto &e : expenses) {
        if (e.name.find(keyword) != string::npos) {
            cout << "ID: " << e.id
                 << " Name: " << e.name
                 << " Amount: Rs. " << e.amount
                 << " Date: " << e.date << endl;
            found = true;
        }
    }

    if (!found)
        cout << "Expense not found.\n";
}

// Delete expense
void deleteExpense() {
    int id;

    cout << "Enter expense ID to delete: ";
    if (!(cin >> id)) {
        cout << "Invalid ID!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    auto it = find_if(expenses.begin(), expenses.end(),
        [id](Expense e) { return e.id == id; });

    if (it != expenses.end()) {
        expenses.erase(it);
        saveExpenses();
        cout << "Expense deleted successfully!\n";
    } else {
        cout << "Expense ID not found.\n";
    }
}

// Main program
int main() {
    int choice;

    cout << "===== SMART DIGITAL EXPENSE TRACKER =====\n";
    cout << "1. Register\n";
    cout << "2. Login\n";

    cout << "Enter choice: ";
    if (!(cin >> choice)) return 0;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice == 1) {
        registerUser();
        cout << "Please login to continue.\n";
    }
    else if (choice != 2) {
        cout << "Invalid choice!\n";
        return 0;
    }

    if (!loginUser())
        return 0;

    loadExpenses();
    loadBudget();

    do {
        cout << "\n===== EXPENSE TRACKER MENU =====\n";
        cout << "1. Add Daily Expense\n";
        cout << "2. View Expense History\n";
        cout << "3. Category-wise Expense\n";
        cout << "4. Monthly Expense Report\n";
        cout << "5. Set Monthly Budget\n";
        cout << "6. Budget Warning\n";
        cout << "7. Expense Prediction\n";
        cout << "8. Search Expense\n";
        cout << "9. Delete Expense\n";
        cout << "10. Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input!\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: addExpense(); break;
            case 2: viewHistory(); break;
            case 3: categoryExpense(); break;
            case 4: monthlyReport(); break;
            case 5: setBudget(); break;
            case 6: budgetWarning(); break;
            case 7: expensePrediction(); break;
            case 8: searchExpense(); break;
            case 9: deleteExpense(); break;
            case 10: cout << "Thank you for using Expense Tracker!\n"; break;
            default: cout << "Invalid choice!\n";
        }

    } while (choice != 10);

    saveExpenses();
    return 0;
}