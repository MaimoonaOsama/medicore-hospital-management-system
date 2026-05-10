#ifndef HOSPITALEXCEPTION_H
#define HOSPITALEXCEPTION_H

#include <iostream>

class HospitalException {
protected:
    char message[200];  

public:
    HospitalException(const char* msg) {
        int i = 0;
        while (msg[i] != '\0' && i < 199) {  
            message[i] = msg[i];
            i++;
        }
        message[i] = '\0'; 
    }

    virtual const char* what() const {
        return message; 
    }
};

#endif