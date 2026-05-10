#include "Doctor.h"
#include "Patient.h"
#include "CharUtils.h"
#include "Validator.h"
#include "FileHandler.h"
#include <iostream>

static bool promptToday(char* buf) 
{
    for (int attempt = 0; attempt < 3; attempt++)
    {
        std::cout << "Enter today's date (DD-MM-YYYY): ";
        std::cin >> buf;
        if (Validator::validateDate(buf)) return true;
        std::cout << "Invalid date. Use format DD-MM-YYYY." << std::endl;
    }
    std::cout << "Too many invalid attempts." << std::endl;
    return false;
}

Doctor::Doctor(int id, const char* name, const char* password,
    const char* spec, float fee)
    : Person(id, name, password), fee(fee) {
    CharUtils::copy(this->specialization, spec, 50);
}


void Doctor::display() const 
{
    std::cout << *this << std::endl;
}

void Doctor::viewTodaysAppointments(Storage<Appointment>& appointments,Storage<Patient>& patients) {
    char today[11];
    if (!promptToday(today)) return;
    int  count = appointments.size();
    bool found = false;
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            if (CharUtils::equals(appointments.getAll()[i].getTimeSlot(),
                appointments.getAll()[j].getTimeSlot()) > 0) {
                Appointment tmp = appointments.getAll()[i];
                appointments.getAll()[i] = appointments.getAll()[j];
                appointments.getAll()[j] = tmp;
            }
        }
    }

    std::cout << std::endl << "Today's Appointments (" << today << "):" << std::endl;
    std::cout << "----------------------------------------------" << std::endl;
    for (int i = 0; i < count; i++)
    {
        Appointment& app = appointments.getAll()[i];
        if (app.getDoctorId() != id)
            continue;
        if (!CharUtils::equals(app.getDate(), today))
            continue;
        char patName[51] = "Unknown";
        for (int j = 0; j < patients.size(); j++)
        {
            if (patients.getAll()[j].getId() == app.getPatientId())
            {
                CharUtils::copy(patName, patients.getAll()[j].getName(), 51);
                break;
            }
        }

        std::cout << "ID: " << app.getAppointmentId()
            << " | Patient: " << patName
            << " | Slot: " << app.getTimeSlot()
            << " | Status: " << app.getStatus()
            << std::endl;
        found = true;
    }

    if (!found)
        std::cout << "No appointments scheduled for today.\n";
}

void Doctor::markAppointmentComplete(Storage<Appointment>& appointments) {
    char today[11];
    if (!promptToday(today)) 
        return;

    bool anyPending = false;
    std::cout << std::endl << "Today's Pending Appointments:" << std::endl;
    for (int i = 0; i < appointments.size(); i++) 
    {
        Appointment& app = appointments.getAll()[i];
        if (app.getDoctorId() == id &&
            CharUtils::equals(app.getDate(), today) &&
            CharUtils::equals(app.getStatus(), "pending"))
        {
            std::cout << "ID: " << app.getAppointmentId()
                << " | Slot: " << app.getTimeSlot() << std::endl;
            anyPending = true;
        }
    }
    if (!anyPending) {
        std::cout << "No pending appointments for today." << std::endl;
        return;
    }
    int apptId;
    std::cout << "Enter Appointment ID: ";
    std::cin >> apptId;
    for (int i = 0; i < appointments.size(); i++)
    {
        Appointment& app = appointments.getAll()[i];
        if (app.getAppointmentId() == apptId &&
            app.getDoctorId() == id &&
            CharUtils::equals(app.getDate(), today) &&
            CharUtils::equals(app.getStatus(), "pending"))
        {
            app.setStatus("completed");
            FileHandler::saveAppointments("appointment.txt", appointments);
            std::cout << "Appointment marked as completed." << std::endl;
            return;
        }
    }
    std::cout << "Invalid appointment ID." << std::endl;
}


void Doctor::markAppointmentNoShow(Storage<Appointment>& appointments,Storage<Bill>& bills)
{
    char today[11];
    if (!promptToday(today)) 
        return;

    bool anyPending = false;
    std::cout << std::endl << "Today's Pending Appointments:" << std::endl;
    for (int i = 0; i < appointments.size(); i++) 
    {
        Appointment& app = appointments.getAll()[i];
        if (app.getDoctorId() == id &&CharUtils::equals(app.getDate(), today) &&CharUtils::equals(app.getStatus(), "pending"))
        {
            std::cout << "ID: " << app.getAppointmentId()
                << " | Slot: " << app.getTimeSlot() << std::endl;
            anyPending = true;
        }
    }
    if (!anyPending) {
        std::cout << "No pending appointments for today." << std::endl;
        return;
    }
    int apptId;
    std::cout << "Enter Appointment ID: ";
    std::cin >> apptId;
    for (int i = 0; i < appointments.size(); i++)
    {
        Appointment& app = appointments.getAll()[i];
        if (app.getAppointmentId() == apptId && app.getDoctorId() == id && CharUtils::equals(app.getDate(), today) &&CharUtils::equals(app.getStatus(), "pending")) {
            app.setStatus("no-show");
            
            for (int j = 0; j < bills.size(); j++)
            {
                if (bills.getAll()[j].getAppointmentId() == apptId)
                {
                    bills.getAll()[j].setStatus("cancelled");
                    break;
                }
            }

            FileHandler::saveAppointments("appointment.txt", appointments);
            FileHandler::saveBills("bill.txt", bills);
            std::cout << "Appointment marked as no-show." << std::endl;
            return;
        }
    }
    std::cout << "Invalid appointment ID." << std::endl;
}


void Doctor::writePrescription(Storage<Prescription>& prescriptions,
    Storage<Appointment>& appointments,
    Storage<Patient>& patients) {
    int apptId;
    std::cout << "Enter Appointment ID: ";
    std::cin >> apptId;
    Appointment* selected = nullptr;
    for (int i = 0; i < appointments.size(); i++)
    {
        Appointment& app = appointments.getAll()[i];
        if (app.getAppointmentId() == apptId &&
            app.getDoctorId() == id &&
            CharUtils::equals(app.getStatus(), "completed"))
        {
            selected = &app;
            break;
        }
    }

    if (!selected)
    {
        std::cout << "Invalid appointment ID." << std::endl;
        return;
    }
    for (int i = 0; i < prescriptions.size(); i++)
    {
        if (prescriptions.getAll()[i].getAppointmentId() == apptId)
        {
            std::cout << "Prescription already written for this appointment.";
            return;
        }
    }
    std::cin.ignore();
    char medicines[500];
    char notes[300];
    std::cout << "Enter medicines (e.g. Paracetamol 500mg;Amoxicillin 250mg): ";
    std::cin.getline(medicines, 500);
    std::cout << "Enter notes (max 300 chars): ";
    std::cin.getline(notes, 300);
    char today[11];
    if (!promptToday(today))
        return;

   
    int maxId = 0;
    for (int i = 0; i < prescriptions.size(); i++)
    {
        if (prescriptions.getAll()[i].getPrescriptionId() > maxId)
            maxId = prescriptions.getAll()[i].getPrescriptionId();
    }
    int newId = maxId + 1;
    Prescription p(newId, apptId, selected->getPatientId(),id, medicines, notes, today);
    prescriptions.add(p);
    FileHandler::savePrescriptions("prescriptions.txt", prescriptions);
    std::cout << "Prescription saved." << std::endl;
}


void Doctor::viewPatientMedicalHistory(int patientId,Storage<Appointment>& appointments, Storage<Prescription>& prescriptions,Storage<Patient>& patients)
{
  
    bool patientExists = false;

    for (int i = 0; i < patients.size(); i++)
    {
        if (patients.getAll()[i].getId() == patientId)
        {
            patientExists = true;
            break;
        }
    }
    if (!patientExists) {
        std::cout << "Access denied. You can only view records of your own patients.\n";
        return;
    }

    bool allowed = false;
    for (int i = 0; i < appointments.size(); i++)
    {
        Appointment& app = appointments.getAll()[i];
        if (app.getPatientId() == patientId &&
            app.getDoctorId() == id &&
            CharUtils::equals(app.getStatus(), "completed"))
        {
            allowed = true;
            break;
        }
    }
    if (!allowed) {
        std::cout << "Access denied. You can only view records of your own patients.\n";
        return;
    }
    Prescription local[100];
    int localCount = 0;
    for (int i = 0; i < prescriptions.size(); i++)
    {
        Prescription& p = prescriptions.getAll()[i];
        if (p.getPatientId() == patientId && p.getDoctorId() == id)
        {
            if (localCount < 100)
                local[localCount++] = p;
        }
    }
    if (localCount == 0) {
        std::cout << "No medical history found." << std::endl;
        return;
    }

    
    for (int i = 0; i < localCount - 1; i++)
    {
        for (int j = i + 1; j < localCount; j++)
        {
            
            if (CharUtils::compareDatesDesc(local[i].getDate(), local[j].getDate()) > 0)
            {
                Prescription tmp = local[i];
                local[i] = local[j];
                local[j] = tmp;
            }
        }
    }

    std::cout << std::endl << "Medical History for Patient ID " << patientId << std::endl;
    std::cout << "----------------------------------------------" << std::endl;
    for (int i = 0; i < localCount; i++)
    {
        std::cout << "Date: " << local[i].getDate()
            << " | Medicines: " << local[i].getMedicines()
            << " | Notes: " << local[i].getNotes()
            << std::endl;
    }
}