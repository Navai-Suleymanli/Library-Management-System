#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include "Book.h"
#include "Patron.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

class FileOperations {
private:
    static const std::string BOOKS_FILE;
    static const std::string PATRONS_FILE;

public:
    static bool saveBooks(const std::vector<Book>& books) {
        std::ofstream file(BOOKS_FILE);
        if (!file) {
            std::cerr << "Error opening file for writing: " << BOOKS_FILE << std::endl;
            return false;
        }

        for (const auto& book : books) {
            file << book.getTitle() << ","
                << book.getAuthor() << ","
                << book.getISBN() << ","
                << book.getAvailableCopies() << std::endl;
        }

        file.close();
        return true;
    }

    static bool loadBooks(std::vector<Book>& books) {
        std::ifstream file(BOOKS_FILE);
        if (!file) {
            std::cerr << "Warning: Could not open file for reading: " << BOOKS_FILE << std::endl;
            return false;
        }

        books.clear();
        std::string line;
        int lineNumber = 0;

        while (std::getline(file, line)) {
            lineNumber++;
            if (line.empty()) continue;

            std::vector<std::string> parts = splitCSV(line);

            if (parts.size() != 4) {
                std::cerr << "WARNING:malformed line in " << BOOKS_FILE << ": " << lineNumber << "," << line << std::endl;
                continue;
            }

            try {
                int copies = std::stoi(parts[3]);
                books.emplace_back(parts[0], parts[1], parts[2], copies);
            }
            catch (const std::exception& e) {
                std::cerr << "WARNING: Invalid snumber of copies in " << BOOKS_FILE << ": " << lineNumber << "," << line << std::endl;
            }
        }

        std::cout << "[DEBUG] Total books loaded: " << books.size() << std::endl;
        file.close();
        return true;
    }

    static bool savePatrons(const std::vector<Patron>& patrons, const std::vector<Book>& books) {
        std::ofstream file(PATRONS_FILE);
        if (!file) {
            std::cerr << "Error opening file for writing: " << PATRONS_FILE << std::endl;
            return false;
        }

        for (const auto& patron : patrons) {
            // Write patron basic info
            file << patron.getName() << "," << patron.getId();

            // Write borrowed book ISBNs
            auto borrowedISBNs = patron.getBorrowedISBNs();
            if (!borrowedISBNs.empty()) {
                file << ",";
                for (size_t i = 0; i < borrowedISBNs.size(); ++i) {
                    file << borrowedISBNs[i];
                    if (i < borrowedISBNs.size() - 1) {
                        file << ";";
                    }
                }
            }

            file << std::endl;
        }

        file.close();
        return true;
    }

    static bool loadPatrons(std::vector<Patron>& patrons, std::vector<Book>& books) {
        std::ifstream file(PATRONS_FILE);
        if (!file) {
            std::cerr << "Warning: Could not open file for reading: " << PATRONS_FILE << std::endl;
            return false;
        }

        patrons.clear();
        std::string line;
        int lineNumber = 0;

        while (std::getline(file, line)) {
            lineNumber++;
            if (line.empty()) continue;

            // Split the line by commas
            std::vector<std::string> parts = splitCSV(line);

            if (parts.size() < 2) {
                std::cerr << "WARNING: Malformed line in " << PATRONS_FILE << ": " << lineNumber << "," << line << std::endl;
                continue;
            }

            // Create the patron
            patrons.emplace_back(parts[0], parts[1]);

            // If there are borrowed books, add them
            if (parts.size() > 2 && !parts[2].empty()) {
                std::vector<std::string> borrowedISBNs = splitString(parts[2], ';');
                for (const auto& isbn : borrowedISBNs) {
                    // Find the book by ISBN
                    AbstractBook* book = findBookByISBN(books, isbn);
                    if (book) {
                        patrons.back().borrowBook(book);
                    }
                }
            }
        }

        std::cout << "[DEBUG] Total patrons loaded: " << patrons.size() << std::endl;
        std::cout << "[DEBUG] All patrons after loading: " << std::endl;
        for (const auto& patron : patrons) {
            std::cout << "  - " << patron.getName() << " (ID: " << patron.getId() << ")" << std::endl;
        }

        file.close();
        return true;
    }

private:
    static std::vector<std::string> splitCSV(const std::string& line) {
        std::vector<std::string> parts;
        std::stringstream ss(line);
        std::string part;

        while (std::getline(ss, part, ',')) {
            // Trim whitespace
            part.erase(0, part.find_first_not_of(" \t"));
            part.erase(part.find_last_not_of(" \t") + 1);
            parts.push_back(part);
        }

        return parts;
    }

    static std::vector<std::string> splitString(const std::string& str, char delimiter) {
        std::vector<std::string> parts;
        std::stringstream ss(str);
        std::string part;

        while (std::getline(ss, part, delimiter)) {
            // Trim whitespace
            part.erase(0, part.find_first_not_of(" \t"));
            part.erase(part.find_last_not_of(" \t") + 1);
            parts.push_back(part);
        }

        return parts;
    }

    static AbstractBook* findBookByISBN(std::vector<Book>& books, const std::string& isbn) {
        for (auto& book : books) {
            if (book.getISBN() == isbn) {
                return &book;
            }
        }
        return nullptr;
    }
};

// Define static constants
const std::string FileOperations::BOOKS_FILE = "books.csv";
const std::string FileOperations::PATRONS_FILE = "patrons.csv";

#endif // FILE_OPERATIONS_H