#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include"CharUtils.h"

class Appointment {
public:
    Appointment(int appointmentId, int patientId, int doctorId, const char* date, const char* timeSlot, const char* status);
    Appointment();
    bool operator==(const Appointment& other) const;
    void display() const;
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
    const char* getTimeSlot() const
    {
        return timeSlot;
    }
    const char* getStatus() const 
    {
        return status; 
    }
    void setStatus(const char* newStatus)
    {
        CharUtils::copy(this->status, newStatus,10);
    }
    float getFee() const { return fee; }
    
    Appointment& operator=(const Appointment& other) {
        if (this != &other)
        {
            appointmentId = other.appointmentId;
            patientId = other.patientId;
            doctorId = other.doctorId;
            CharUtils::copy(this->date, other.date, sizeof(this->date));
            CharUtils::copy(this->timeSlot, other.timeSlot, sizeof(this->timeSlot));
            CharUtils::copy(this->status, other.status, sizeof(this->status));
        }
        return *this;
    }
private:
    int appointmentId;
    int patientId;
    int doctorId;
    char date[20]; 
    char timeSlot[15];
    char status[10];
    float fee;
    void copyDate(const char* source);
    void copyTimeSlot(const char* source);
    void copyStatus(const char* source);
};

#endif 