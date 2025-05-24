#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "AbstractBook.h"
#include "Patron.h"
#include "Loggable.h"
#include "TimeStamped.h"
#include <iostream>

enum TransactionType { BORROW, RETURN };

class Transaction : public Loggable, public TimeStamped {
public:
    Transaction(AbstractBook* book, Patron* patron, TransactionType type)
        : book(book), patron(patron), type(type) {
    }

    void display() const {
        std::cout << "Transaction: " << (type == BORROW ? "Borrow" : "Return")
            << "\nBook: " << book->getTitle()
            << "\nPatron: " << patron->getName()
            << "\nTime: " << getTimeStamp() << "\n";
    }

    // Override the log method.
    void log() const override {
        std::cout << "[LOG] " << (type == BORROW ? "Borrow" : "Return")
            << " transaction for book: " << book->getTitle()
            << " by patron: " << patron->getName() << "\n";
    }

private:
    AbstractBook* book;
    Patron* patron;
    TransactionType type;
};

#endif // TRANSACTION_H