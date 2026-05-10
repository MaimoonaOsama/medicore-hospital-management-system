#ifndef ADMIN_H
#define ADMIN_H

#include "Person.h"
#include "Doctor.h"
#include "Patient.h"
#include "Storage.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "FileHandler.h"
#include "Validator.h"
#include <iostream>

class Admin : public Person {
public:
    Admin(int id = 0, const char* name = " ", const char* password=" ") : Person(id, name, password) {}

    void display() const override {
        std::cout << "Admin ID: " << id << std::endl;
        std::cout << "Admin Name: " << name << std::endl;
    }

    const char* getPassword() const override
    {
        return password;
    }
    const int getid() {
        return id;
    }

    bool operator==(const Person& other) const override
    {
        const Admin& otherAdmin = dynamic_cast<const Admin&>(other);
        return this->id == otherAdmin.id;
    }

    void addDoctor(Storage<Doctor>& doctors);

    void removeDoctor(Storage<Doctor>& doctors, Storage<Appointment>& appointments);

    void addPatient(Storage<Patient>& patients);

    void removePatient(Storage<Patient>& patients, Storage<Appointment>& appointments, Storage<Bill>& bills);

    void viewAllPatients(Storage<Patient>& patients);

    void viewAllDoctors(Storage<Doctor>& doctors);

    void viewAllAppointments(Storage<Appointment>& appointments);

    void viewUnpaidBills(Storage<Bill>& bills);

    void dischargePatient(Storage<Patient>& patients, Storage<Appointment>& appointments, Storage<Bill>& bills);

    void viewSecurityLog();

    void generateDailyReport(Storage<Appointment>& appointments, Storage<Bill>& bills);

private:
    
    int getMaxDoctorId(Storage<Doctor>& doctors);
    int getMaxPatientId(Storage<Patient>& patients);
    void printDoctorList(Storage<Doctor>& doctors);
    void printPatientList(Storage<Patient>& patients);
};

#endif