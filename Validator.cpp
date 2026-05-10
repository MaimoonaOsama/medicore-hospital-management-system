#include "Validator.h"

bool Validator::validateID(int id) {
    return id > 0;  
}

bool Validator::validateDate(const char* date) {
    // Ensure that the date is in DD-MM-YYYY format (10 characters long)
    if (date[2] != '-' || date[5] != '-' || date[10] != '\0') return false;

   
    for (int i = 0; i < 2; ++i) {
        if (date[i] < '0' || date[i] > '9') return false;  // Check date part
    }
    for (int i = 3; i < 5; ++i) {
        if (date[i] < '0' || date[i] > '9') return false;  // Check month part
    }
    for (int i = 6; i < 10; ++i) {
        if (date[i] < '0' || date[i] > '9') return false;  // Check year part
    }
    return true;
}

bool Validator::validateTimeSlot(const char* timeSlot) {
    const char validSlots[8][6] = {
        "09:00", "10:00", "11:00", "12:00",
        "13:00", "14:00", "15:00", "16:00"
    };

    for (int i = 0; i < 8; i++) {
        bool match = true;
        int j = 0;
        while (validSlots[i][j] != '\0' || timeSlot[j] != '\0') {
            if (validSlots[i][j] != timeSlot[j]) { match = false; break; }
            j++;
        }
        if (match) return true;
    }
    return false;
}

bool Validator::validateContactNumber(const char* contactNumber)
{
    for (int i = 0; i < 10; ++i) {
        if (contactNumber[i] < '0' || contactNumber[i] > '9') return false;  
    }
    return true;
}

bool Validator::validatePassword(const char* password) {
    
    int length = 0;
    while (password[length] != '\0') {
        length++;
    }
    return length >= 8; 
}

bool Validator::validateFloat(float value)
{
    return value > 0.0f;  
}

bool Validator::validateAge(int age)
{
    return age >= 1 && age <= 150;
}

bool Validator::validateGender(const char* gender) {
   
    if (gender[0] == 'M' || gender[0] == 'm') {
        if (gender[1] == 'a' || gender[1] == 'A') {
            if (gender[2] == 'l' || gender[2] == 'L') {
                if (gender[3] == 'e' || gender[3] == 'E') {
                    return true;
                }
            }
        }
    }
    if (gender[0] == 'F' || gender[0] == 'f') {
        if (gender[1] == 'e' || gender[1] == 'E') {
            if (gender[2] == 'm' || gender[2] == 'M') {
                if (gender[3] == 'a' || gender[3] == 'A') {
                    if (gender[4] == 'l' || gender[4] == 'L') {
                        if (gender[5] == 'e' || gender[5] == 'E') {
                            return true;
                        }
                    }
                }
            }
        }
    }  
    return false;
}