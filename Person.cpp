#include "Person.h"

void Person::copyName(const char* source) {
    int i = 0;
    while (source[i] != '\0' && i < 49) {
        name[i] = source[i];
        i++;
    }
    name[i] = '\0';
}

bool Person::compareName(const char* other) const {
    int i = 0;
    while (name[i] != '\0' && other[i] != '\0') {
        if (name[i] != other[i]) {
            return false;
        }
        i++;
    }
    return name[i] == other[i];  
}

Person::Person(int id, const char* name, const char* password) {
    this->id = id;
    copyName(name); 
    int i = 0;
    while (password[i] != '\0' && i < 49) {
        this->password[i] = password[i];
        i++;
    }
    this->password[i] = '\0';  
}

Person::~Person() {}

