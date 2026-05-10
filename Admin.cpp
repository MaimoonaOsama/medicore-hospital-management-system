#include "Admin.h"
#include "FileHandler.h"
#include "Validator.h"
#include "CharUtils.h"
#include "Appointment.h"
#include "Bill.h"
#include <iostream>
#include <fstream>

int Admin::getMaxDoctorId(Storage<Doctor>& doctors) {
    int maxId = 0;
    for (int i = 0; i < doctors.size(); ++i) {
        if (doctors.getAll()[i].getId() > maxId) {
            maxId = doctors.getAll()[i].getId();
        }
    }
    return maxId;
}

int Admin::getMaxPatientId(Storage<Patient>& patients) {
    int maxId = 0;
    for (int i = 0; i < patients.size(); ++i) {
        if (patients.getAll()[i].getId() > maxId) {
            maxId = patients.getAll()[i].getId();
        }
    }
    return maxId;
}

void Admin::printPatientList(Storage<Patient>& patients) {
    std::cout << "Patient List:" << std::endl;
    for (int i = 0; i < patients.size(); i++) {
        std::cout << patients.getAll()[i].getId() << " | "
            << patients.getAll()[i].getName() << " | "
            << patients.getAll()[i].getAge() << " | "
            << patients.getAll()[i].getGender() << " | "
            << patients.getAll()[i].getContact() << " | "
            << patients.getAll()[i].getBalance() << std::endl;
    }
}

void Admin::printDoctorList(Storage<Doctor>& doctors) {
    std::cout << "Doctor List:" << std::endl;
    for (int i = 0; i < doctors.size(); i++) {
        std::cout << doctors.getAll()[i].getId() << " | "
            << doctors.getAll()[i].getName() << " | "
            << doctors.getAll()[i].getSpecialization() << " | "
            << doctors.getAll()[i].getFee() << std::endl;
    }
}

// Add Doctor
void Admin::addDoctor(Storage<Doctor>& doctors) {
    char name[50], specialization[50], contact[12], password[50];
    float fee;

    std::cout << "Enter Doctor's Name: ";
    std::cin.getline(name, 50);
    std::cout << "Enter Specialization: ";
    std::cin.getline(specialization, 50);
    std::cout << "Enter Contact (11 digits): ";
    std::cin >> contact;
    if (!Validator::validateContactNumber(contact)) {
        std::cout << "Invalid Contact!" << std::endl;
        return;
    }
    std::cout << "Enter Password (Min 6 characters): ";
    std::cin >> password;
    if (!Validator::validatePassword(password)) {
        std::cout << "Invalid Password!" << std::endl;
        return;
    }
    std::cout << "Enter Consultation Fee: ";
    std::cin >> fee;
    if (fee <= 0) {
        std::cout << "Invalid Fee!" << std::endl;
        return;
    }
    int newId = getMaxDoctorId(doctors) + 1;
    Doctor newDoctor(newId, name, password, specialization, fee);
    doctors.add(newDoctor);
    FileHandler::saveDoctors("doctors.txt", doctors);
    std::cout << "Doctor added successfully. ID: " << newId << std::endl;
}

//  Remove Doctor
void Admin::removeDoctor(Storage<Doctor>& doctors, Storage<Appointment>& appointments) {
    int doctorId;
    printDoctorList(doctors);
    std::cout << "Enter Doctor ID to remove: ";
    std::cin >> doctorId;

    bool hasPendingAppointments = false;
    for (int i = 0; i < appointments.size(); ++i) {
        if (appointments.getAll()[i].getDoctorId() == doctorId &&
            CharUtils::compare(appointments.getAll()[i].getStatus(), "pending")) {
            hasPendingAppointments = true;
            break;
        }
    }

    if (hasPendingAppointments) {
        std::cout << "Cannot remove doctor with pending appointments. Cancel or reassign them first." << std::endl;
        return;
    }

    doctors.removeById(doctorId);
    FileHandler::saveDoctors("doctors.txt", doctors);
    std::cout << "Doctor removed." << std::endl;
}

// Add Patient
void Admin::addPatient(Storage<Patient>& patients) {
    char name[50], gender, contact[12], password[50];
    int age;
    float balance;

    std::cout << "Enter Patient's Name: ";
    std::cin.getline(name, 50);
    std::cout << "Enter Age: ";
    std::cin >> age;
    std::cin.ignore();
    std::cout << "Enter Gender (M/F): ";
    std::cin >> gender;
    std::cout << "Enter Contact (11 digits): ";
    std::cin >> contact;
    std::cout << "Enter Password (Only 8 characters): ";
    std::cin >> password;
    std::cout << "Enter Initial Balance: ";
    std::cin >> balance;

    int newId = getMaxPatientId(patients) + 1;
    Patient newPatient(newId, name, password,age,gender,contact, balance);
    patients.add(newPatient);

    FileHandler::savePatients("patients.txt", patients);
    std::cout << "Patient added successfully. ID: " << newId << std::endl;
}

// Remove Patient
void Admin::removePatient(Storage<Patient>& patients, Storage<Appointment>& appointments, Storage<Bill>& bills) {
    int patientId;
    printPatientList(patients);
    std::cout << "Enter Patient ID to remove: ";
    std::cin >> patientId;

    bool hasPendingAppointments = false;
    for (int i = 0; i < appointments.size(); ++i) {
        if (appointments.getAll()[i].getPatientId() == patientId &&
            CharUtils::compare(appointments.getAll()[i].getStatus(), "pending")) {
            hasPendingAppointments = true;
            break;
        }
    }

    bool hasUnpaidBills = false;
    for (int i = 0; i < bills.size(); ++i) {
        if (bills.getAll()[i].getPatientId() == patientId &&
            CharUtils::compare(bills.getAll()[i].getStatus(), "unpaid")) {
            hasUnpaidBills = true;
            break;
        }
    }

    if (hasPendingAppointments) {
        std::cout << "Cannot remove patient with pending appointments." << std::endl;
        return;
    }

    if (hasUnpaidBills) {
        std::cout << "Cannot remove patient with unpaid bills." << std::endl;
        return;
    }

    patients.removeById(patientId);
    FileHandler::savePatients("patients.txt", patients);
    std::cout << "Patient removed successfully." << std::endl;
}

//  View All Patients
void Admin::viewAllPatients(Storage<Patient>& patients)
{
    printPatientList(patients);
}

//  View All Doctors
void Admin::viewAllDoctors(Storage<Doctor>& doctors)
{
    printDoctorList(doctors);
}

//  View All Appointments
void Admin::viewAllAppointments(Storage<Appointment>& appointments) {
    FileHandler::loadAppointments("appointment.txt", appointments);
    std::cout << "ID | PatientID | DoctorID | Date | Time Slot | Status" << std::endl;
    for (int i = 0; i < appointments.size(); ++i) {
        std::cout <<appointments.getAll()[i].getAppointmentId() << " | "
            << appointments.getAll()[i].getPatientId() << " | "
            << appointments.getAll()[i].getDoctorId() << " | "
            << appointments.getAll()[i].getDate() << " | "
            << appointments.getAll()[i].getTimeSlot() << " | "
            << appointments.getAll()[i].getStatus() << std::endl;
    }
}

// View Unpaid Bills
void Admin::viewUnpaidBills(Storage<Bill>& bills) {
    FileHandler::loadBills("bill.txt", bills);
    std::cout << "Bill ID | Patient ID | Amount (PKR)" << std::endl;
    for (int i = 0; i < bills.size(); ++i) {
        if (CharUtils::compare(bills.getAll()[i].getStatus(), "unpaid")==0) {
            std::cout << bills.getAll()[i].getBillId() << " | "
                << bills.getAll()[i].getPatientId() << " | "
                << bills.getAll()[i].getAmount()  << std::endl;
        }
    }
}

//  Discharge Patient
void Admin::dischargePatient(Storage<Patient>& patients, Storage<Appointment>& appointments, Storage<Bill>& bills)
{
    int patientId;
    std::cout << "Enter Patient ID to discharge: ";
    std::cin >> patientId;

    bool hasUnpaidBills = false;
    for (int i = 0; i < bills.size(); ++i) {
        if (bills.getAll()[i].getPatientId() == patientId &&
            CharUtils::compare(bills.getAll()[i].getStatus(), "unpaid")) {
            hasUnpaidBills = true;
            break;
        }
    }

    bool hasPendingAppointments = false;
    for (int i = 0; i < appointments.size(); ++i) {
        if (appointments.getAll()[i].getPatientId() == patientId &&
            CharUtils::compare(appointments.getAll()[i].getStatus(), "pending")) {
            hasPendingAppointments = true;
            break;
        }
    }

    if (hasUnpaidBills) {
        std::cout << "Cannot discharge patient with unpaid bills." << std::endl;
        return;
    }

    if (hasPendingAppointments) {
        std::cout << "Cannot discharge patient with pending appointments." << std::endl;
        return;
    }

   
    FileHandler::archivePatientRecord(patientId, patients, appointments, bills, "discharged.txt");
    std::cout << "Patient discharged and archived successfully." << std::endl;
}

//  View Security Log
void Admin::viewSecurityLog() {
    std::ifstream logFile("security_log.txt");
    if (!logFile.is_open()) {
        std::cout << "No security events logged." << std::endl;
        return;
    }

    char line[300];
    bool hasContent = false;
    while (logFile.getline(line, 300)) {
        std::cout << line << std::endl;
        hasContent = true;
    }
    logFile.close();

    if (!hasContent) {
        std::cout << "No security events logged." << std::endl;
    }
}

//Generate Daily Report
void Admin::generateDailyReport(Storage<Appointment>& appointments, Storage<Bill>& bills) {
    int totalAppointments = 0, pending = 0, completed = 0, noShow = 0, cancelled = 0;
    float revenue = 0;

    for (int i = 0; i < appointments.size(); ++i) {
        if (CharUtils::compare(appointments.getAll()[i].getStatus(), "pending")==0) {
            pending++;
        }
        else if (CharUtils::compare(appointments.getAll()[i].getStatus(), "completed")==0) {
            completed++;

            revenue += bills.getAll()[i].getAmount();
           
        }
        else if (CharUtils::compare(appointments.getAll()[i].getStatus(), "no-show")==0) {
            noShow++;
        }
        else if (CharUtils::compare(appointments.getAll()[i].getStatus(), "cancelled")==0) {
            cancelled++;
        }
        totalAppointments++;
    }

    float totalUnpaid = 0;
    for (int i = 0; i < bills.size(); ++i) {
      
        if (CharUtils::compare(bills.getAll()[i].getStatus(), "unpaid") == 0)
        {
            totalUnpaid += bills.getAll()[i].getAmount();
        }
    }

    std::cout << std::endl<<"--- Daily Report ---" << std::endl;
    std::cout << "Total appointments today: " << totalAppointments
        << " (Pending: " << pending
        << " Completed: " << completed
        << " No-show: " << noShow
        << " Cancelled: " << cancelled << ")" << std::endl;
    std::cout << "Revenue collected today (paid bills): PKR " << revenue << std::endl;
    std::cout << "Patients with outstanding unpaid bills: PKR " << totalUnpaid << std::endl;
}