#ifndef LOGGABLE_H
#define LOGGABLE_H

#include <iostream>
#include <string>

class Loggable {
public:
    virtual ~Loggable() {}
    virtual void log() const = 0;
};

#endif // LOGGABLE_H