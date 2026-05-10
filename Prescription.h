#ifndef PRESCRIPTION_H
#define PRESCRIPTION_H

#include"CharUtils.h"

class Prescription {
public:
    Prescription(int prescriptionId, int appointmentId, int patientId, int doctorId, const char* date, const char* medicines, const char* notes);
    Prescription() : prescriptionId(0), appointmentId(0), patientId(0), doctorId(0), medicines(""), notes("") {}
    bool operator==(const Prescription& other) const;
    void display() const;
    int getPrescriptionId() const
    {
        return prescriptionId; 
    }
    int getAppointmentId() const 
    {
        return appointmentId;
    }
    int getPatientId() const 
    {
        return patientId;
    }
    int getDoctorId() const
    {
        return doctorId;
    }
    const char* getDate() const
    {
        return date;
    }
    const char* getMedicines() const
    {
        return medicines;
    }
    const char* getNotes() const
    { 
        return notes;
    }
    const char* getDoctorName() const 
    {
        return doctorName;
    }
    Prescription& operator=(const Prescription& other) 
    {
        if (this != &other) { 
           
            CharUtils::copy(this->date, other.date, sizeof(this->date));
            CharUtils::copy(this->medicines, other.medicines, sizeof(this->medicines));
            CharUtils::copy(this->notes, other.notes, sizeof(this->notes));
            CharUtils::copy(this->doctorName, other.doctorName, sizeof(this->doctorName));
            this->prescriptionId = other.prescriptionId;
            this->appointmentId = other.appointmentId;
            this->patientId = other.patientId;
            this->doctorId = other.doctorId;
        }
        return *this;
    }
private:
    int prescriptionId;
    int appointmentId;
    int patientId;
    int doctorId;
    char date[20]; 
    char medicines[500]; 
    char notes[300];
    char doctorName[50];
    void copyDate(const char* source);
    void copyMedicines(const char* source);
    void copyNotes(const char* source);
};

#endif 