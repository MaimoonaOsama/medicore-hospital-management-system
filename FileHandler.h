#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "Storage.h"
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"

class Admin;

class FileHandler {
public:
    
    static void archivePatientRecord(int patientId, Storage<Patient>& patients,Storage<Appointment>& appointments,Storage<Bill>& bills, const char* archiveFile = "discharged.txt");
    static bool loadPatients(const char* filename, Storage<Patient>& patients);
    static bool loadadmins(const char* filename, Storage<Admin>& admins);
    static bool loadDoctors(const char* filename, Storage<Doctor>& doctors);
    static bool loadAppointments(const char* filename, Storage<Appointment>& appointments);
    static bool loadBills(const char* filename, Storage<Bill>& bills);
    static bool loadPrescriptions(const char* filename, Storage<Prescription>& prescriptions);

    static bool savePatients(const char* filename, const Storage<Patient>& patients);
    static bool saveAdmins(const char* filename, const Storage<Admin>& admins);
    static bool saveDoctors(const char* filename, const Storage<Doctor>& doctors);
    static bool saveAppointments(const char* filename, const Storage<Appointment>& appointments);
    static bool saveBills(const char* filename, const Storage<Bill>& bills);
    static bool savePrescriptions(const char* filename, const Storage<Prescription>& prescriptions);
};

#endif 