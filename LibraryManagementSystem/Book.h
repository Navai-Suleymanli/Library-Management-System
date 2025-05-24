#ifndef BOOK_H
#define BOOK_H

#include "AbstractBook.h"
#include <iostream>

class Book : public AbstractBook {
private:
    std::string title;
    std::string author;
    std::string isbn;
    int availableCopies;

public:
    Book() : title(""), author(""), isbn(""), availableCopies(0) {}
    Book(const std::string& title, const std::string& author,
        const std::string& isbn, int copies)
        : title(title), author(author), isbn(isbn), availableCopies(copies) {
    }

    void display() const override {
        std::cout << "Title: " << title
            << "\nAuthor: " << author
            << "\nISBN: " << isbn
            << "\nAvailable Copies: " << availableCopies << "\n";
    }

    std::string getTitle() const override { return title; }
    std::string getAuthor() const override { return author; }
    std::string getISBN() const override { return isbn; }
    int getAvailableCopies() const { return availableCopies; }

    bool borrowBook() {
        if (availableCopies > 0) {
            availableCopies--;
            return true;
        }
        return false;
    }

    void returnBook() {
        availableCopies++;
    }

    friend std::ostream& operator<<(std::ostream& os, const Book& b) {
        os << "Title: " << b.title
            << "\nAuthor: " << b.author
            << "\nISBN: " << b.isbn
            << "\nAvailable Copies: " << b.availableCopies;
        return os;
    }
};

#endif // BOOK_H