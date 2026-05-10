#include "Patient.h"
#include "Doctor.h"         
#include "CharUtils.h"
#include "Validator.h"
#include "FileHandler.h"
#include "HospitalException.h"
#include "SlotUnavailableException.h"
#include "InvalidInputException.h"
#include "InsufficientFundsException.h"
#include <iostream>
#include <cstdio>

//Constructor 
Patient::Patient(int id, const char* name, const char* password, int age, char gender, const char* contact, float balance) : Person(id, name, password), balance(balance), age(age), gender(gender)
{
    CharUtils::copy(this->contact, contact, 20);
}

void Patient::display() const {
    std::cout << *this << std::endl;
}

//all the functionalities required for patient classs
//book appointment
void Patient::bookAppointment(Storage<Doctor>& doctors, Storage<Appointment>& appointments, Storage<Bill>& bills) 
{
    char spec[50];
    std::cout << "Enter specialization to search (e.g. Cardiology): ";
    std::cin >> spec;

    bool anyDoc = false;

    for (int i = 0; i < doctors.size(); i++) {
        if (CharUtils::equalsIgnoreCase(doctors.getAll()[i].getSpecialization(), spec))
        {
            std::cout << "ID: " << doctors.getAll()[i].getId()
                << " | Name: " << doctors.getAll()[i].getName()
                << " | Fee: PKR " << doctors.getAll()[i].getFee()
                << std::endl;
            anyDoc = true;
        }
    }

    if (!anyDoc) {
        std::cout << "No doctors available for that specialization." << std::endl;
        return;
    }

    int docId;
    std::cout << "Enter Doctor ID: ";
    std::cin >> docId;
    Doctor* selectedDoc = nullptr;
    for (int i = 0; i < doctors.size(); i++) {
        if (doctors.getAll()[i].getId() == docId) {
            selectedDoc = &doctors.getAll()[i];
            break;
        }
    }
    if (!selectedDoc) {
        std::cout << "Doctor not found." << std::endl;
        return;
    }
    char date[20];
    bool validDate = false;
    for (int attempt = 0; attempt < 3; attempt++) {
        std::cout << "Enter date (DD-MM-YYYY): ";
        std::cin >> date;
        if (Validator::validateDate(date)) {
            validDate = true;
            break;
        }
        std::cout << "Invalid date. Use format DD-MM-YYYY." << std::endl;
    }

    if (!validDate) {
        return;
    }

    const char slots[8][6] = {
        "09:00", "10:00", "11:00", "12:00",
        "13:00", "14:00", "15:00", "16:00"
    };

    std::cout << std::endl << "Available slots for Dr. " << selectedDoc->getName() << " on " << date << ":" << std::endl;
    for (int i = 0; i < 8; i++) {
        bool taken = false;

        for (int j = 0; j < appointments.size(); j++) {
            Appointment& a = appointments.getAll()[j];
            if (a.getDoctorId() == docId &&
                CharUtils::equals(a.getDate(), date) &&
                CharUtils::equals(a.getTimeSlot(), slots[i]) &&
                !CharUtils::equals(a.getStatus(), "cancelled")) {
                taken = true;
                break;
            }
        }
        if (!taken) {
            std::cout << "  " << slots[i] << std::endl;
        }
    }

    char selectedSlot[10];
    while (true) {
        std::cout << "Enter time slot (e.g. 09:00): ";
        std::cin >> selectedSlot;
        bool isValid = false;

        for (int i = 0; i < 8; i++) {
            if (CharUtils::equals(selectedSlot, slots[i])) {
                isValid = true;
                break;
            }
        }
        if (!isValid) {
            std::cout << "Invalid slot. Please choose from the list." << std::endl;
            continue;
        }
        bool taken = false;
        for (int j = 0; j < appointments.size(); j++) {
            Appointment& a = appointments.getAll()[j];
            if (a.getDoctorId() == docId &&
                CharUtils::equals(a.getDate(), date) &&
                CharUtils::equals(a.getTimeSlot(), selectedSlot) &&
                !CharUtils::equals(a.getStatus(), "cancelled")) {
                taken = true;
                break;
            }
        }
        if (taken) {
            try {
                throw SlotUnavailableException(selectedSlot);
            }
            catch (HospitalException& e) {
                std::cout << e.what() << std::endl;

                std::cout << "Available slots:" << std::endl;

                for (int i = 0; i < 8; i++) {
                    bool t2 = false;

                    for (int j = 0; j < appointments.size(); j++)
                    {
                        Appointment& a = appointments.getAll()[j];

                        if (a.getDoctorId() == docId &&
                            CharUtils::equals(a.getDate(), date) &&
                            CharUtils::equals(a.getTimeSlot(), slots[i]) &&
                            !CharUtils::equals(a.getStatus(), "cancelled"))
                        {
                            t2 = true;
                            break;
                        }
                    }

                    if (!t2) {
                        std::cout << "  " << slots[i] << std::endl;
                    }
                }
            }

            continue;
        }

        break;
    }

    float fee = selectedDoc->getFee();
    if (this->getBalance() < fee) {
        try
        {
            throw InsufficientFundsException("Insufficient Funds");
        }
        catch (HospitalException& e)
        {
            std::cout << e.what() << std::endl;
            return;
        }
    }

    int maxApptId = 0;

    for (int i = 0; i < appointments.size(); i++)
    {
        if (appointments.getAll()[i].getAppointmentId() > maxApptId)
        {
            maxApptId = appointments.getAll()[i].getAppointmentId();
        }
    }

    int newApptId = maxApptId + 1;
    Appointment newAppt(newApptId, id, docId, date, selectedSlot, "pending");
    appointments.add(newAppt);

    int maxBillId = 0;
    for (int i = 0; i < bills.size(); i++)
    {
        if (bills.getAll()[i].getBillId() > maxBillId)
        {
            maxBillId = bills.getAll()[i].getBillId();
        }
    }

    int newBillId = maxBillId + 1;
    Bill newBill(newBillId, id, newApptId, fee, "unpaid");
    bills.add(newBill);
    *this -= fee;
    FileHandler::saveAppointments("appointment.txt", appointments);
    FileHandler::saveBills("bill.txt", bills);
    std::cout << "Appointment booked successfully. Appointment ID: " << newApptId << std::endl;
}

//cancel the appointment
void Patient::cancelAppointment(Storage<Appointment>& appointments,Storage<Bill>& bills, Storage<Doctor>& doctors,Storage<Patient>& patients) {
    // pernding appointiments for *this patient
    bool any = false;
    std::cout << std::endl<<"Your Pending Appointments:" << std::endl;
    for (int i = 0; i < appointments.size(); i++)
    {
        Appointment& a = appointments.getAll()[i];
        if (a.getPatientId() == id &&
            CharUtils::equals(a.getStatus(), "pending"))
        {
            char docName[51] = "Unknown";
            for (int j = 0; j < doctors.size(); j++)
            {
                if (doctors.getAll()[j].getId() == a.getDoctorId())
                {
                    CharUtils::copy(docName, doctors.getAll()[j].getName(), 51);
                    break;
                }
            }
            std::cout << "ID: " << a.getAppointmentId() << " | " << docName << " | Date: " << a.getDate() << " | Slot: " << a.getTimeSlot() << std::endl;
            any = true;
        }
    }

    if (!any) {
        std::cout << "You have no pending appointments.\n";
        return;
    }
    int apptId;
    std::cout << "Enter Appointment ID to cancel: ";
    std::cin >> apptId;

   
    Appointment* appt = nullptr;
    for (int i = 0; i < appointments.size(); i++) {
        Appointment& a = appointments.getAll()[i];
        if (a.getAppointmentId() == apptId &&
            a.getPatientId() == id &&
            CharUtils::equals(a.getStatus(), "pending")) {
            appt = &a;
            break;
        }
    }

    if (!appt) {
        std::cout << "Invalid appointment ID." << std::endl;
        return;
    }

    Bill* bill = nullptr;
    for (int i = 0; i < bills.size(); i++) {
        if (bills.getAll()[i].getAppointmentId() == apptId &&
            CharUtils::equals(bills.getAll()[i].getStatus(), "unpaid")) 
        {
            bill = &bills.getAll()[i];
            break;
        }
    }
    float refund = bill ? bill->getAmount() : 0.0f;
    appt->setStatus("cancelled");
    if (bill) bill->setStatus("cancelled");
    *this += refund;

    for (int i = 0; i < patients.size(); i++)
    {
        if (patients.getAll()[i].getId() == id)
        {
            patients.getAll()[i].setBalance(balance);
            break;
        }
    }

    FileHandler::saveAppointments("appointment.txt", appointments);
    FileHandler::saveBills("bill.txt", bills);
    FileHandler::savePatients("patients.txt", patients);

    std::cout << "Appointment cancelled. PKR " << refund << " refunded to your balance." << std::endl;
}

//viewing appointment 
void Patient::viewAppointments(Storage<Appointment>& appointments,Storage<Doctor>& doctors) {
    // this patients appointment in an array
    Appointment local[MAX_PATIENT_APPOINTMENTS];
    int localCount = 0;
    for (int i = 0; i < appointments.size() && localCount < MAX_PATIENT_APPOINTMENTS; i++)
    {
        if (appointments.getAll()[i].getPatientId() == id)
            local[localCount++] = appointments.getAll()[i];
    }

    if (localCount == 0) {
        std::cout << "No appointments found.\n";
        return;
    }

    // Bubble sort ascending by date
    for (int i = 0; i < localCount - 1; i++) {
        for (int j = i + 1; j < localCount; j++) {
            if (CharUtils::compareDatesAsc(local[i].getDate(), local[j].getDate()) > 0) {
                Appointment tmp = local[i];
                local[i] = local[j];
                local[j] = tmp;
            }
        }
    }
    std::cout << std::endl<< "Your Appointments:" << std::endl;
    std::cout << "----------------------------------------------" << std::endl;
    for (int i = 0; i < localCount; i++)
    {
        char docName[51] = "Unknown";
        char docSpec[51] = "Unknown";
        for (int j = 0; j < doctors.size(); j++)
        {
            if (doctors.getAll()[j].getId() == local[i].getDoctorId())
            {
                CharUtils::copy(docName, doctors.getAll()[j].getName(), 51);
                CharUtils::copy(docSpec, doctors.getAll()[j].getSpecialization(), 51);
                break;
            }
        }
        std::cout << "ID: " << local[i].getAppointmentId()<< " |" << docName << " | Spec: " << docSpec << " | Date: " << local[i].getDate()<< " | Slot: " << local[i].getTimeSlot()<< " | Status: " << local[i].getStatus() << std::endl;
    }
}


// viewing medical records
void Patient::viewMedicalRecords(Storage<Prescription>& prescriptions,Storage<Appointment>& appointments,Storage<Doctor>& doctors) {
    Prescription local[MAX_PATIENT_PRESCRIPTIONS];
    int localCount = 0;
    for (int i = 0; i < prescriptions.size() && localCount < MAX_PATIENT_PRESCRIPTIONS; i++)
    {
        if (prescriptions.getAll()[i].getPatientId() == id)
            local[localCount++] = prescriptions.getAll()[i];
    }

    if (localCount == 0) {
        std::cout << "No medical records found.\n";
        return;
    }
    //sorting desceinding by date
    for (int i = 0; i < localCount - 1; i++) {
        for (int j = i + 1; j < localCount; j++) {
            if (CharUtils::compareDatesDesc(local[i].getDate(), local[j].getDate()) > 0) {
                Prescription tmp = local[i];
                local[i] = local[j];
                local[j] = tmp;
            }
        }
    }

    std::cout << std::endl<< "Your Medical Records:" << std::endl;
    std::cout << "----------------------------------------------" << std::endl;
    for (int i = 0; i < localCount; i++) {
       //finding doctor by appointment
        char docName[51] = "Unknown";
        for (int j = 0; j < doctors.size(); j++)
        {
            if (doctors.getAll()[j].getId() == local[i].getDoctorId())
            {
                CharUtils::copy(docName, doctors.getAll()[j].getName(), 51);
                break;
            }
        }
        std::cout << "Date: " << local[i].getDate()<< " | Dr. " << docName << " | Medicines: " << local[i].getMedicines() << " | Notes: " << local[i].getNotes()<<std::endl;
    }
}

// viewBills
void Patient::viewBills(Storage<Bill>& bills) {
    float totalUnpaid = 0.0f;
    bool  any = false;

    std::cout << std::endl<<"Your Bills:" << std::endl;
    std::cout << "----------------------------------------------" << std::endl;
    for (int i = 0; i < bills.size(); i++)
    {
        Bill& b = bills.getAll()[i];
        if (b.getPatientId() != id) 
            continue;

        std::cout << "Bill ID: " << b.getBillId() << " | Appt ID: " << b.getAppointmentId() << " | Amount: PKR " << b.getAmount() << " | Status: " << b.getStatus() << std::endl;

        if (CharUtils::equals(b.getStatus(), "unpaid"))
            totalUnpaid += b.getAmount();
             any = true;
    }

    if (!any)
    {
        std::cout << "No bills found." << std::endl;
        return;
    }
    std::cout << "Total outstanding (unpaid): PKR " << totalUnpaid << std::endl;
}


// payBill

void Patient::payBill(Storage<Bill>& bills,
    Storage<Patient>& patients) {
    // Show unpaid bills
    bool any = false;
    std::cout << std::endl<< "Unpaid Bills:" << std::endl;
    for (int i = 0; i < bills.size(); i++)
    {
        Bill& b = bills.getAll()[i];
        if (b.getPatientId() == id &&
            CharUtils::equals(b.getStatus(), "unpaid"))
        {
            std::cout << "Bill ID: " << b.getBillId()
                << " | Amount: PKR " << b.getAmount() << std::endl;
            any = true;
        }
    }

    if (!any) {
        std::cout << "No unpaid bills." << std::endl;
        return;
    }

    int billId;
    std::cout << "Enter Bill ID to pay: ";
    std::cin >> billId;

    Bill* bill = nullptr;
    for (int i = 0; i < bills.size(); i++) {
        if (bills.getAll()[i].getBillId() == billId &&
            bills.getAll()[i].getPatientId() == id &&
            CharUtils::compare(bills.getAll()[i].getStatus(), "unpaid")) {
            bill = &bills.getAll()[i];
            break;
        }
    }

    if (!bill) {
        std::cout << "Invalid bill ID." << std::endl;
        return;
    }

    if (balance < bill->getAmount())
    {
        try {
            throw InsufficientFundsException("Insufficient Funds");
        }
        catch (HospitalException& e)
        {
            std::cout << e.what() << std::endl;
        }
        return;
    }

    *this -= bill->getAmount();
    bill->setStatus("paid");
    for (int i = 0; i < patients.size(); i++) {
        if (patients.getAll()[i].getId() == id) {
            patients.getAll()[i].setBalance(balance);
            break;
        }
    }

    FileHandler::saveBills("bill.txt", bills);
    FileHandler::savePatients("patients.txt", patients);
    std::cout << "Bill paid successfully. Remaining balance: PKR " << balance << std::endl;
}


// top up balance
void Patient::topUpBalance(Storage<Patient>& patients) {
    float amount;
    int attempts = 0;
    while (attempts < 3) {
        std::cout << "Enter amount to add (PKR): ";
        if (!(std::cin >> amount) || amount <= 0.0f) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            try {
                throw InvalidInputException("Amount must be a positive number.");
            }
            catch (HospitalException& e) 
            {
                std::cout << e.what() << std::endl;
            }
            attempts++;
            continue;
        }

        *this += amount;

        for (int i = 0; i < patients.size(); i++)
        {
            if (patients.getAll()[i].getId() == id)
            {
                patients.getAll()[i].setBalance(balance);
                break;
            }
        }

        FileHandler::savePatients("patients.txt", patients);
        std::cout << "Balance updated. New balance: PKR " << balance << std::endl;
        return;
    }

    std::cout << "Too many invalid attempts. Returning to menu."<<std::endl;
}