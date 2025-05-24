
#ifndef INVENTORY_H
#define INVENTORY_H

#include "AbstractBook.h"
#include "Book.h"
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "Utils.h"

class Inventory {
public:
    ~Inventory() {
        for (auto book : books) {
            delete book;
        }
    }

    bool hasISBN(const std::string& isbn) const {
        for (auto book : books) {
            if (book->getISBN() == isbn) return true;
        }
        return false;
    }

    void addBook(AbstractBook* book) {
        books.push_back(book);
    }

    bool remmoveBook(const std::string& isbn) {
        auto it = std::remove_if(books.begin(), books.end(),
            [&](AbstractBook* b) { return b->getISBN() == isbn; });

        if (it != books.end()) {
            for (auto itr = it; itr != books.end(); ++itr) {
                delete* itr;
            }
            books.erase(it, books.end());
            return true;
        }
        return false;
    }

    AbstractBook* searchBookByISBN(const std::string& isbn) {
        for (auto book : books) {
            if (book->getISBN() == isbn) return book;
        }
        return nullptr;
    }

    void listBooks() const {
        for (auto book : books) {
            book->display();
            std::cout << "---------------------\n";
        }
    }

    void saveToFile(const std::string& filename) const {
        std::ofstream ofs(filename);
        if (!ofs) throw std::runtime_error("Error opening file: " + filename);

        for (auto book : books) {
            Book* b = dynamic_cast<Book*>(book);
            if (b) {
                ofs << b->getTitle() << ","
                    << b->getAuthor() << ","
                    << b->getISBN() << ","
                    << b->getAvailableCopies() << "\n";
            }
        }
        ofs.close();
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream ifs(filename);
        if (!ifs) throw std::runtime_error("Error opening file: " + filename);

        // Clear existing inventory
        for (auto book : books) delete book;
        books.clear();

        std::string line;
        while (std::getline(ifs, line)) {
            std::istringstream iss(line);
            std::string title, author, isbn, copiesStr;

            if (std::getline(iss, title, ',') &&
                std::getline(iss, author, ',') &&
                std::getline(iss, isbn, ',') &&
                std::getline(iss, copiesStr)) {

                // Trim whitespace from fields
                title = Utils::trim(title);
                author = Utils::trim(author);
                isbn = Utils::trim(isbn);
                copiesStr = Utils::trim(copiesStr);

                try {
                    int copies = std::stoi(copiesStr);
                    books.push_back(new Book(title, author, isbn, copies));
                }
                catch (const std::exception& e) {
                    std::cerr << "Invalid data in line: " << line << "\n";
                }
            }
        }
        ifs.close();
    }

private:
    std::vector<AbstractBook*> books;
};

#endif // INVENTORY_H