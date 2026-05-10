#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <iostream>

class Validator {
public:
    static bool validateID(int id);
    static bool validateDate(const char* date);  // DD-MM-YYYY format
    static bool validateTimeSlot(const char* timeSlot);  // HH:MM format
    static bool validateContactNumber(const char* contactNumber);  // 10 digit format
    static bool validatePassword(const char* password);  // Password with at least 8 characters
    static bool validateFloat(float value);
    static bool validateAge(int age);  // Age should be between 1 and 150
    static bool validateGender(const char* gender);  // M or F
};

#endif 