#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

// ANSI color codes for colored output
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

struct Book {
    int id;
    string title;
    string author;
    bool isBorrowed = false;
    int borrowedBy = -1; // member id
    int borrowDate = -1; // day number
};

struct Member {
    int id;
    string name;
};

vector<Book> books;
vector<Member> members;

int currentDay = 0; // Simulate days

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pressEnterToContinue() {
    cout << YELLOW << "Press Enter to continue..." << RESET;
    cin.ignore();
    cin.get();
}

Book* findBookById(int id) {
    for (auto& book : books)
        if (book.id == id) return &book;
    return nullptr;
}

Member* findMemberById(int id) {
    for (auto& member : members)
        if (member.id == id) return &member;
    return nullptr;
}

void addBook() {
    clearScreen();
    Book b;
    cout << CYAN << "=== Add Book ===" << RESET << endl;
    cout << "Enter book ID: ";
    while (!(cin >> b.id)) {
        cout << RED << "Invalid input. Enter a number: " << RESET;
        cin.clear(); cin.ignore(10000, '\n');
    }
    cin.ignore();
    cout << "Enter title: "; getline(cin, b.title);
    cout << "Enter author: "; getline(cin, b.author);
    books.push_back(b);
    cout << GREEN << "Book added!" << RESET << endl;
    pressEnterToContinue();
}

void addMember() {
    clearScreen();
    Member m;
    cout << CYAN << "=== Add Member ===" << RESET << endl;
    cout << "Enter member ID: ";
    while (!(cin >> m.id)) {
        cout << RED << "Invalid input. Enter a number: " << RESET;
        cin.clear(); cin.ignore(10000, '\n');
    }
    cin.ignore();
    cout << "Enter name: "; getline(cin, m.name);
    members.push_back(m);
    cout << GREEN << "Member added!" << RESET << endl;
    pressEnterToContinue();
}

void listBooks() {
    cout << BOLD << left << setw(6) << "ID" << setw(30) << "Title" << setw(20) << "Author" << setw(12) << "Status" << RESET << endl;
    for (const auto& book : books) {
        cout << left << setw(6) << book.id
             << setw(30) << book.title
             << setw(20) << book.author
             << setw(12) << (book.isBorrowed ? (string(RED) + "Borrowed" + RESET) : (string(GREEN) + "Available" + RESET))
             << endl;
    }
}

void searchBooks() {
    clearScreen();
    int choice;
    cout << CYAN << "=== Search Books ===" << RESET << endl;
    cout << "Search by: 1. Title 2. Author\n";
    while (!(cin >> choice) || (choice != 1 && choice != 2)) {
        cout << RED << "Invalid choice. Enter 1 or 2: " << RESET;
        cin.clear(); cin.ignore(10000, '\n');
    }
    cin.ignore();
    string query;
    cout << "Enter search term: ";
    getline(cin, query);
    bool found = false;
    cout << endl;
    for (const auto& book : books) {
        if ((choice == 1 && book.title.find(query) != string::npos) ||
            (choice == 2 && book.author.find(query) != string::npos)) {
            cout << left << setw(6) << book.id
                 << setw(30) << book.title
                 << setw(20) << book.author
                 << setw(12) << (book.isBorrowed ? (string(RED) + "Borrowed" + RESET) : (string(GREEN) + "Available" + RESET))
                 << endl;
            found = true;
        }
    }
    if (!found) cout << YELLOW << "No books found." << RESET << endl;
    pressEnterToContinue();
}

void borrowBook() {
    clearScreen();
    cout << CYAN << "=== Borrow Book ===" << RESET << endl;
    listBooks();
    int bookId, memberId;
    cout << "\nEnter book ID: ";
    while (!(cin >> bookId)) {
        cout << RED << "Invalid input. Enter a number: " << RESET;
        cin.clear(); cin.ignore(10000, '\n');
    }
    cout << "Enter member ID: ";
    while (!(cin >> memberId)) {
        cout << RED << "Invalid input. Enter a number: " << RESET;
        cin.clear(); cin.ignore(10000, '\n');
    }
    Book* book = findBookById(bookId);
    Member* member = findMemberById(memberId);
    if (!book) { cout << RED << "Book not found." << RESET << endl; pressEnterToContinue(); return; }
    if (!member) { cout << RED << "Member not found." << RESET << endl; pressEnterToContinue(); return; }
    if (book->isBorrowed) { cout << RED << "Book already borrowed." << RESET << endl; pressEnterToContinue(); return; }
    book->isBorrowed = true;
    book->borrowedBy = memberId;
    book->borrowDate = currentDay;
    cout << GREEN << "Book borrowed successfully!" << RESET << endl;
    pressEnterToContinue();
}

void returnBook() {
    clearScreen();
    cout << CYAN << "=== Return Book ===" << RESET << endl;
    listBooks();
    int bookId;
    cout << "\nEnter book ID: ";
    while (!(cin >> bookId)) {
        cout << RED << "Invalid input. Enter a number: " << RESET;
        cin.clear(); cin.ignore(10000, '\n');
    }
    Book* book = findBookById(bookId);
    if (!book) { cout << RED << "Book not found." << RESET << endl; pressEnterToContinue(); return; }
    if (!book->isBorrowed) { cout << YELLOW << "Book is not borrowed." << RESET << endl; pressEnterToContinue(); return; }
    book->isBorrowed = false;
    book->borrowedBy = -1;
    book->borrowDate = -1;
    cout << GREEN << "Book returned successfully!" << RESET << endl;
    pressEnterToContinue();
}

void listOverdueBooks() {
    clearScreen();
    cout << CYAN << "=== Overdue Books ===" << RESET << endl;
    const int borrowPeriod = 14;
    bool found = false;
    for (const auto& book : books) {
        if (book.isBorrowed && (currentDay - book.borrowDate) > borrowPeriod) {
            cout << left << setw(6) << book.id
                 << setw(30) << book.title
                 << setw(20) << book.author
                 << "Borrowed by: " << book.borrowedBy
                 << ", Days overdue: " << (currentDay - book.borrowDate - borrowPeriod) << endl;
            found = true;
        }
    }
    if (!found) cout << GREEN << "No overdue books." << RESET << endl;
    pressEnterToContinue();
}

void advanceDay() {
    clearScreen();
    int days;
    cout << CYAN << "=== Advance Day ===" << RESET << endl;
    cout << "Enter number of days to advance: ";
    while (!(cin >> days) || days < 1) {
        cout << RED << "Invalid input. Enter a positive number: " << RESET;
        cin.clear(); cin.ignore(10000, '\n');
    }
    currentDay += days;
    cout << GREEN << "Current day is now: " << currentDay << RESET << endl;
    pressEnterToContinue();
}

void showMenu() {
    clearScreen();
    cout << BOLD << CYAN << "=== Library Management System ===" << RESET << endl;
    cout << "Current Day: " << currentDay << endl;
    cout << "1. Add Book\n";
    cout << "2. Add Member\n";
    cout << "3. Search Books\n";
    cout << "4. Borrow Book\n";
    cout << "5. Return Book\n";
    cout << "6. List Overdue Books\n";
    cout << "7. Advance Day\n";
    cout << "0. Exit\n";
    cout << "Enter choice: ";
}

int main() {
    int choice;
    do {
        showMenu();
        while (!(cin >> choice)) {
            cout << RED << "Invalid input. Enter a number: " << RESET;
            cin.clear(); cin.ignore(10000, '\n');
        }
        cin.ignore();
        switch (choice) {
            case 1: addBook(); break;
            case 2: addMember(); break;
            case 3: searchBooks(); break;
            case 4: borrowBook(); break;
            case 5: returnBook(); break;
            case 6: listOverdueBooks(); break;
            case 7: advanceDay(); break;
            case 0: clearScreen(); cout << GREEN << "Goodbye!" << RESET << endl; break;
            default: cout << RED << "Invalid choice." << RESET << endl; pressEnterToContinue();
        }
    } while (choice != 0);
    return 0;
}
