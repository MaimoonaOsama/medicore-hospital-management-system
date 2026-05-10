#ifndef BILL_H
#define BILL_H
#include"CharUtils.h"

class Bill {
public:
    Bill(int billId, int patientId, int appointmentId, float amount, const char* status);
    Bill() : billId(0), appointmentId(0), amount(0.0) {}
    void display() const;
    int getBillId() const
    {
        return billId;
    }
    int getPatientId() const 
    {
        return patientId;
    }
    int getAppointmentId() const 
    {
        return appointmentId;
    }
    float getAmount() const 
    {
        return amount;
    }
    const char* getStatus() const 
    {
        return status; 
    }
    bool operator==(const Bill& other) const
    {
        return this->billId == other.billId; 
    }
    void setStatus(const char* status) 
    {
        CharUtils::copy(this->status, status, sizeof(this->status));
    }
private:
    int billId;
    int patientId;
    int appointmentId;
    float amount;
    char status[20]; 
    void copyStatus(const char* source);
   
};

#endif 