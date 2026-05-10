#include "Bill.h"
#include <iostream>
using namespace std;

Bill::Bill(int billId, int patientId, int appointmentId, float amount, const char* status) {
    this->billId = billId;
    this->patientId = patientId;
    this->appointmentId = appointmentId;
    this->amount = amount;
    copyStatus(status);
  
}

void Bill::copyStatus(const char* source) {
    int i = 0;
    while (source[i] != '\0' && i < 20) {
        status[i] = source[i];
        i++;
    }
    status[i] = '\0';  
}


void Bill::display() const {
    cout << "Bill ID: " << billId
        << ", Patient ID: " << patientId
        << ", Appointment ID: " << appointmentId
        << ", Amount: PKR " << amount
        << ", Status: " << status << std::endl;
}