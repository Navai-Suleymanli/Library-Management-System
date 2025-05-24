#ifndef ABSTRACT_BOOK_H
#define ABSTRACT_BOOK_H

#include <string>

class AbstractBook {
public:
    virtual ~AbstractBook() = default;
    virtual void display() const = 0;
    virtual std::string getTitle() const = 0;
    virtual std::string getAuthor() const = 0;
    virtual std::string getISBN() const = 0;
};

#endif // ABSTRACT_BOOK_H