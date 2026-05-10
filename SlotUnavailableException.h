#ifndef SLOTUNAVAILABLEEXCEPTION_H
#define SLOTUNAVAILABLEEXCEPTION_H

#include "HospitalException.h"

class SlotUnavailableException : public HospitalException {
public:

    SlotUnavailableException(const char* msg) : HospitalException(msg) {}

    const char* what() const override\
    {
        return message;
    }
};

#endif 