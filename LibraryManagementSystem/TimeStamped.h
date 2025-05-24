#ifndef TIMESTAMPED_H
#define TIMESTAMPED_H

#include <ctime>
#include <sstream>
#include <string>

class TimeStamped {
public:
    TimeStamped() {
        time(&timestamp);
    }
    std::string getTimeStamp() const {
        std::stringstream ss;
        char buffer[26];
        ctime_s(buffer, sizeof(buffer), &timestamp);
        ss << buffer;
        return ss.str();
    }
private:
    time_t timestamp;
};


#endif
