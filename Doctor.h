
#ifndef DOCTOR_H
#define DOCTOR_H

#include "Person.h"
#include "CharUtils.h"
#include "Storage.h"
#include "Appointment.h"
#include "Prescription.h"
#include "Bill.h"
#include <iostream>


class Patient;

class Doctor : public Person {
public:
    Doctor(int id = 0,
        const char* name = "",
        const char* password = "",
        const char* spec = "",
        float fee = 0.0f);

   
    bool operator==(const Person& other) const override {
        const Doctor& d = dynamic_cast<const Doctor&>(other);
        return this->id == d.id;
    }

    friend std::ostream& operator<<(std::ostream& os, const Doctor& d) {
        os << "Doctor ID: " << d.id
            << " | Name: " << d.name
            << " | Specialization: " << d.specialization
            << " | Fee: PKR " << d.fee;
        return os;
    }
    void display() const override;

    void viewTodaysAppointments(Storage<Appointment>& appointments,
        Storage<Patient>& patients);

    void markAppointmentComplete(Storage<Appointment>& appointments);

    void markAppointmentNoShow(Storage<Appointment>& appointments,
        Storage<Bill>& bills);

    void writePrescription(Storage<Prescription>& prescriptions,
        Storage<Appointment>& appointments,
        Storage<Patient>& patients);

    void viewPatientMedicalHistory(int patientId,
        Storage<Appointment>& appointments,
        Storage<Prescription>& prescriptions,
        Storage<Patient>& patients);

    void setSpecialization(const char* spec)
    {
        CharUtils::copy(this->specialization, spec, 50);
    }
    void setFee(float f) { fee = f; }

    int getId()  const 
    {
        return id; 
    }
    const char* getName()  const 
    {
        return name;
    }
    const char* getPassword()  const 
    
    { return password; 
    }
    const char* getSpecialization() const
    {
        return specialization;
    }
    float getFee() const
    {
        return fee;
    }

private:
    char  specialization[50];
    float fee;
};

#endif 