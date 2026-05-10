#include "Appointment.h"
#include <iostream>

Appointment::Appointment(int appointmentId, int patientId, int doctorId, const char* date, const char* timeSlot, const char* status) {
    this->appointmentId = appointmentId;
    this->patientId = patientId;
    this->doctorId = doctorId;
    copyDate(date);
    copyTimeSlot(timeSlot);
    copyStatus(status);
}
Appointment::Appointment() : appointmentId(0), patientId(0), doctorId(0) {
    copyDate(""); 
    copyTimeSlot("");  
    copyStatus("Pending"); 
}
void Appointment::copyDate(const char* source) {
    int i = 0;
    while (source[i] != '\0' && i < 10) {
        date[i] = source[i];
        i++;
    }
    date[i] = '\0';
}

void Appointment::copyTimeSlot(const char* source) {
    int i = 0;
    while (source[i] != '\0' && i < 5) {
        timeSlot[i] = source[i];
        i++;
    }
    timeSlot[i] = '\0'; 
}

void Appointment::copyStatus(const char* source) {
    int i = 0;
    while (source[i] != '\0' && i < 9) {
        status[i] = source[i];
        i++;
    }
    status[i] = '\0';
}

bool Appointment::operator==(const Appointment& other) const {
    return (appointmentId == other.appointmentId);
}

void Appointment::display() const {
    std::cout << "Appointment ID: " << appointmentId
        << ", Patient ID: " << patientId
        << ", Doctor ID: " << doctorId
        << ", Date: " << date
        << ", Time Slot: " << timeSlot
        << ", Status: " << status << std::endl;
}