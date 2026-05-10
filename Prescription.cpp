#include "Prescription.h"
#include <iostream>

Prescription::Prescription(int prescriptionId, int appointmentId, int patientId, int doctorId, const char* date, const char* medicines, const char* notes) {
    this->prescriptionId = prescriptionId;
    this->appointmentId = appointmentId;
    this->patientId = patientId;
    this->doctorId = doctorId;
    copyDate(date);
    copyMedicines(medicines);
    copyNotes(notes);
}

void Prescription::copyDate(const char* source) {
    int i = 0;
    while (source[i] != '\0' && i < 10) {
        date[i] = source[i];
        i++;
    }
    date[i] = '\0';  
}

void Prescription::copyMedicines(const char* source) {
    int i = 0;
    while (source[i] != '\0' && i < 499) {
        medicines[i] = source[i];
        i++;
    }
    medicines[i] = '\0';
}

void Prescription::copyNotes(const char* source) {
    int i = 0;
    while (source[i] != '\0' && i < 299) {
        notes[i] = source[i];
        i++;
    }
    notes[i] = '\0'; 
}

bool Prescription::operator==(const Prescription& other) const {
    return (prescriptionId == other.prescriptionId);
}

void Prescription::display() const {
    std::cout << "Prescription ID: " << prescriptionId
        << ", Appointment ID: " << appointmentId
        << ", Patient ID: " << patientId
        << ", Doctor ID: " << doctorId
        << ", Date: " << date
        << ", Medicines: " << medicines
        << ", Notes: " << notes << std::endl;
}