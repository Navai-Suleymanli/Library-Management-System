#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <limits>
#include <iostream>

class Utils {
public:
    static void clearInputBuffer() {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    static std::string trim(const std::string& str) {
        size_t start = str.find_first_not_of(" \t");
        size_t end = str.find_last_not_of(" \t");
        return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
    }

    template<typename T>
    static T getValidatedInput(const std::string& prompt) {
        T value;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value) break;
            std::cerr << "Invalid input. Please try again.\n";
            clearInputBuffer();
        }
        clearInputBuffer();
        return value;
    }

    // Specialization for string input
    template<>
    static std::string getValidatedInput<std::string>(const std::string& prompt) {
        std::string value;
        while (true) {
            std::cout << prompt;
            if (std::getline(std::cin, value)) {
                value = trim(value);
                if (!value.empty()) break;
            }
            std::cerr << "Invalid input. Please try again.\n";
            clearInputBuffer();
        }
        return value;
    }
};

#endif // UTILS_H