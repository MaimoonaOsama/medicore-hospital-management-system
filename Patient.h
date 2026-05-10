#ifndef PATIENT_H
#define PATIENT_H

#include "Person.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "Storage.h"
#include <iostream>

class Doctor;

#define MAX_PATIENT_APPOINTMENTS  100
#define MAX_PATIENT_BILLS         100
#define MAX_PATIENT_PRESCRIPTIONS 100

class Patient : public Person {
public:
    Patient(int id = 0,const char* name = "",const char* password = "", int age = 0,char gender = 'M',const char* contact = "",float       balance = 0.0f);
    bool operator==(const Person& other)
        const override {
        const Patient& p = dynamic_cast<const Patient&>(other);
        return this->id == p.id;
    }

    Patient& operator+=(float amount)
    {
        balance += amount;
        return *this; 
    }
    Patient& operator-=(float amount)
    {
        balance -= amount;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const Patient& p)
    {
        os << "Patient ID: " << p.id
            << " | Name: " << p.name
            << " | Balance: PKR " << p.balance;
        return os;
    }

    void display() const override;
    void bookAppointment(Storage<Doctor>& doctors,Storage<Appointment>& appointments,Storage<Bill>& bills);

    void cancelAppointment(Storage<Appointment>& appointments,Storage<Bill>& bills,Storage<Doctor>& doctors,Storage<Patient>& patients);

    void viewAppointments(Storage<Appointment>& appointments,Storage<Doctor>& doctors);

    void viewMedicalRecords(Storage<Prescription>& prescriptions,Storage<Appointment>& appointments,Storage<Doctor>& doctors);

    void viewBills(Storage<Bill>& bills);

    void payBill(Storage<Bill>& bills,Storage<Patient>& patients);

    void topUpBalance(Storage<Patient>& patients);

    
    int getId()  const
    {
        return id; 
    }
    const char* getName()  
        const 
    {
        return name; 
    }
    const char* getPassword()const
    {
        return password;
    }
    int getAge()  const
    {
        return age;
    }
    char getGender()  const
    {
        return gender;
    }
    const char* getContact() const
    { 
        return contact;
    }
    float getBalance() const
    { 
        return balance; 
    }

    void setBalance(float b)
    {
        balance = b;
    }
    void setAge(int a)
    {
        age = a;
    }
    void setGender(char g)
    {
        gender = g; 
    }
    void setContact(const char* c)
    {
        CharUtils::copy(contact, c, 20);
    }

private:
    float balance;
    int   age;
    char  gender;
    char  contact[20];
};

#include "CharUtils.h"

#endif 