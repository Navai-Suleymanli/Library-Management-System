#include "Book.h"
#include "Patron.h"
#include "FileOperations.h"
#include <iostream>
#include <vector>
#include <string>
#include <limits>

// Global vectors to store books and patrons
std::vector<Book> books;
std::vector<Patron> patrons;

// Function prototypes
void displayMenu();
void addBook();
void removeBook();
void listAllBooks();
void registerPatron();
void borrowBook();
void returnBook();
void fileOperationsMenu();
void listPatrons();

int main() {
    // Load data at startup
    FileOperations::loadBooks(books);
    FileOperations::loadPatrons(patrons, books);

    int choice = 0;
    bool running = true;

    while (running) {
        displayMenu();
        std::cout << "Enter choice: ";
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        std::cin.ignore(); // Clear the input buffer

        switch (choice) {
        case 1: addBook(); break;
        case 2: removeBook(); break;
        case 3: listAllBooks(); break;
        case 4: registerPatron(); break;
        case 5: borrowBook(); break;
        case 6: returnBook(); break;
        case 7: listPatrons(); break;
        case 8: fileOperationsMenu(); break;
        case 0:
            std::cout << "Exiting program. Saving data...\n";
            FileOperations::saveBooks(books);
            FileOperations::savePatrons(patrons, books);
            running = false;
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}

void displayMenu() {
    std::cout << "\n=== Library Management System ===\n";
    std::cout << "1. Add Book\n";
    std::cout << "2. Remove Book\n";
    std::cout << "3. List All Books\n";
    std::cout << "4. Register Patron\n";
    std::cout << "5. Borrow Book\n";
    std::cout << "6. Return Book\n";
    std::cout << "7. List All Patrons\n";
    std::cout << "8. File Operations\n";
    std::cout << "0. Exit\n";
}

void addBook() {
    std::string title, author, isbn;
    int copies;

    std::cout << "Enter book title: ";
    std::getline(std::cin, title);

    std::cout << "Enter author name: ";
    std::getline(std::cin, author);

    std::cout << "Enter ISBN: ";
    std::getline(std::cin, isbn);

    // Check if ISBN already exists
    for (const auto& book : books) {
        if (book.getISBN() == isbn) {
            std::cout << "Error: A book with this ISBN already exists.\n";
            return;
        }
    }

    std::cout << "Enter number of copies: ";
    std::cin >> copies;
    std::cin.ignore();

    if (copies < 0) {
        std::cout << "Error: Number of copies cannot be negative.\n";
        return;
    }

    books.emplace_back(title, author, isbn, copies);
    std::cout << "Book added successfully!\n";
}

void removeBook() {
    if (books.empty()) {
        std::cout << "There are no books in the library.\n";
        return;
    }

    std::string isbn;
    std::cout << "Enter ISBN of book to remove: ";
    std::getline(std::cin, isbn);

    for (auto it = books.begin(); it != books.end(); ++it) {
        if (it->getISBN() == isbn) {
            // Check if any patron has borrowed this book
            bool borrowed = false;
            for (const auto& patron : patrons) {
                if (patron.hasBorrowed(isbn)) {
                    borrowed = true;
                    break;
                }
            }

            if (borrowed) {
                std::cout << "Error: This book is currently borrowed by one or more patrons.\n";
                return;
            }

            books.erase(it);
            std::cout << "Book removed successfully!\n";
            return;
        }
    }

    std::cout << "Book with ISBN " << isbn << " not found.\n";
}

void listAllBooks() {
    if (books.empty()) {
        std::cout << "There are no books in the library.\n";
        return;
    }

    std::cout << "\n=== Library Boooks ===\n";
    for (const auto& book : books) {
        std::cout << book << "\n----------------------\n";
    }
}

void registerPatron() {
    std::string name, id;

    std::cout << "Enter patron name: ";
    std::getline(std::cin, name);

    std::cout << "Enter patron ID: ";
    std::getline(std::cin, id);

    // Check if ID already exists
    for (const auto& patron : patrons) {
        if (patron.getId() == id) {
            std::cout << "Error: A patron with this ID already exists.\n";
            return;
        }
    }

    patrons.emplace_back(name, id);
    std::cout << "Patron registered successfully!\n";
}

void borrowBook() {
    if (books.empty()) {
        std::cout << "There are no books in the library.\n";
        return;
    }

    if (patrons.empty()) {
        std::cout << "There are no registered patrons.\n";
        return;
    }

    std::string patronId, isbn;

    std::cout << "Enter patron ID: ";
    std::getline(std::cin, patronId);

    Patron* selectedPatron = nullptr;
    for (auto& patron : patrons) {
        if (patron.getId() == patronId) {
            selectedPatron = &patron;
            break;
        }
    }

    if (!selectedPatron) {
        std::cout << "Patron with ID " << patronId << " not found.\n";
        return;
    }

    std::cout << "Enter ISBN of book to borrow: ";
    std::getline(std::cin, isbn);

    Book* selectedBook = nullptr;
    for (auto& book : books) {
        if (book.getISBN() == isbn) {
            selectedBook = &book;
            break;
        }
    }

    if (!selectedBook) {
        std::cout << "Book with ISBN " << isbn << " not found.\n";
        return;
    }

    if (selectedPatron->hasBorrowed(isbn)) {
        std::cout << "This patron has already borrowed this book.\n";
        return;
    }

    if (selectedBook->borrowBook()) {
        selectedPatron->borrowBook(selectedBook);
        std::cout << "Book borrowed successfully!\n";
    }
    else {
        std::cout << "No copies available for borrowing.\n";
    }
}

void returnBook() {
    if (patrons.empty()) {
        std::cout << "There are no registered patrons.\n";
        return;
    }

    std::string patronId, isbn;

    std::cout << "Enter patron ID: ";
    std::getline(std::cin, patronId);

    Patron* selectedPatron = nullptr;
    for (auto& patron : patrons) {
        if (patron.getId() == patronId) {
            selectedPatron = &patron;
            break;
        }
    }

    if (!selectedPatron) {
        std::cout << "Patron with ID " << patronId << " not found.\n";
        return;
    }

    // List borrowed books
    std::cout << "Borrowed books by " << selectedPatron->getName() << ":\n";
    selectedPatron->listBorrowedBooks();

    std::cout << "Enter ISBN of book to return: ";
    std::getline(std::cin, isbn);

    if (!selectedPatron->hasBorrowed(isbn)) {
        std::cout << "This patron has not borrowed a book with ISBN " << isbn << ".\n";
        return;
    }

    Book* selectedBook = nullptr;
    for (auto& book : books) {
        if (book.getISBN() == isbn) {
            selectedBook = &book;
            break;
        }
    }

    if (selectedBook) {
        selectedBook->returnBook();
        selectedPatron->returnBook(selectedBook);
        std::cout << "Book returned successfully!\n";
    }
}

void listPatrons() {
    if (patrons.empty()) {
        std::cout << "There are no registered patrons.\n";
        return;
    }

    std::cout << "\n=== Registered Patrons ===\n";
    for (const auto& patron : patrons) {
        std::cout << "Name: " << patron.getName() << "\n";
        std::cout << "ID: " << patron.getId() << "\n";
        std::cout << "Borrowed Books:\n";
        patron.listBorrowedBooks();
        std::cout << "----------------------\n";
    }
}

void fileOperationsMenu() {
    int choice;
    std::cout << "\nFile Operations:\n";
    std::cout << "1. Save All Data\n";
    std::cout << "2. Load All Data\n";
    std::cout << "Enter choice: ";
    std::cin >> choice;
    std::cin.ignore();

    switch (choice) {
    case 1:
        if (FileOperations::saveBooks(books) &&
            FileOperations::savePatrons(patrons, books)) {
            std::cout << "Data saved successfully!\n";
        }
        else {
            std::cout << "Error saving data.\n";
        }
        break;
    case 2:
        if (FileOperations::loadBooks(books) &&
            FileOperations::loadPatrons(patrons, books)) {
            std::cout << "Data loaded successfully!\n";
        }
        else {
            std::cout << "Error loading data.\n";
        }
        break;
    default:
        std::cout << "Invalid choice.\n";
    }
}