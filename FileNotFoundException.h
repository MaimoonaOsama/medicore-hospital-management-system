#ifndef FILENOTFOUNDEXCEPTION_H
#define FILENOTFOUNDEXCEPTION_H

#include "HospitalException.h"

class FileNotFoundException : public HospitalException {
public:
    FileNotFoundException(const char* msg) : HospitalException(msg) {}

    const char* what() const override
    {
        return message;  
    }
};

#endif 