#ifndef PATRON_H
#define PATRON_H

#include "AbstractBook.h"
#include <string>
#include <vector>
#include <iostream>

class Patron {
private:
    std::string name;
    std::string patronId;
    std::vector<AbstractBook*> borrowedBooks;

public:
    Patron() : name(""), patronId("") {}
    Patron(const std::string& name, const std::string& patronId)
        : name(name), patronId(patronId) {
    }

    std::string getName() const { return name; }
    std::string getId() const { return patronId; }

    void borrowBook(AbstractBook* book) {
        borrowedBooks.push_back(book);
    }

    void returnBook(AbstractBook* book) {
        auto it = std::find(borrowedBooks.begin(), borrowedBooks.end(), book);
        if (it != borrowedBooks.end()) {
            borrowedBooks.erase(it);
        }
    }

    bool hasBorrowed(const std::string& isbn) const {
        for (auto book : borrowedBooks) {
            if (book->getISBN() == isbn) return true;
        }
        return false;
    }

    std::vector<std::string> getBorrowedISBNs() const {
        std::vector<std::string> isbns;
        for (auto book : borrowedBooks) {
            isbns.push_back(book->getISBN());
        }
        return isbns;
    }

    void listBorrowedBooks() const {
        std::cout << "Borrowed Books for " << name << ":\n";
        for (auto book : borrowedBooks) {
            book->display();
            std::cout << "---------------------\n";
        }
    }
};

#endif // PATRON_H