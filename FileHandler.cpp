#include "FileHandler.h"
#include <iostream>
#include <fstream>

static void stripCR(char* buf) {
    int len = 0;
    while (buf[len]) len++;
    if (len > 0 && buf[len - 1] == '\r')
        buf[len - 1] = '\0';
}
// Load Patients
bool FileHandler::loadPatients(const char* filename, Storage<Patient>& patients) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    int id, age;
    char name[50], gender[6], contact[20], password[50];
    float balance;

    while (file >> id) {
        file.get();                       
        file.getline(name, 50, ',');
        file >> age;
        file.get();                       
        file.getline(gender, 3, ',');
        file.getline(contact, 20, ',');
        file.getline(password, 50, ',');
        stripCR(password);
        file >> balance;
        file.ignore(1000, '\n');          
        patients.add(Patient(id, name, password, age, gender[0], contact, balance));
    }

    file.close();
    return true;
}

bool FileHandler::loadadmins(const char* filename, Storage<Admin>& admins) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    int id;
    char name[50], password[50];
  

    while (file >> id) {
        file.get(); 
        file.getline(name, 50, ',');
        file.getline(password, 50, '\n');
        stripCR(name);     
        stripCR(password);
        admins.add(Admin(id, name, password));
    }

    file.close();
    return true;
}

// Load Doctors
bool FileHandler::loadDoctors(const char* filename, Storage<Doctor>& doctors) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    int id;
    char name[50], password[50], specialization[50];
    float fee;

    while (file >> id) {
        file.get();                       
        file.getline(name, 50, ',');
        file.getline(password, 50, ',');
        stripCR(password);
        file.getline(specialization, 50, ',');
        file >> fee;
        file.ignore(1000, '\n');          
        doctors.add(Doctor(id, name, password, specialization, fee));
    }

    file.close();
    return true;
}

// Load Appointments
bool FileHandler::loadAppointments(const char* filename, Storage<Appointment>& appointments) {
    appointments.clear();
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    int appointmentId, patientId, doctorId;
    char date[20], timeSlot[20], status[10];
    char temp[20];

    while (file.getline(temp, 20, ','))
    {  
        appointmentId = atoi(temp);
        file.getline(temp, 20, ',');      
        patientId = atoi(temp);

        file.getline(temp, 20, ',');      
        doctorId = atoi(temp);

        file.getline(date, 20, ',');      

        file.getline(timeSlot, 20, ',');   

        file.getline(status, 10, '\n');   

        appointments.add(Appointment(appointmentId, patientId, doctorId, date, timeSlot, status));
    }

    file.close();
    return true;
}

// Load Bills
bool FileHandler::loadBills(const char* filename, Storage<Bill>& bills) {
    bills.clear();
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    int billId, patientId, appointmentId;
    float amount;
    char status[20];

    while (file >> billId) {
        file.get(); 
        file >> patientId;
        file.get(); 
        file >> appointmentId;
        file.get();  
        file >> amount;
        file.get(); 
        file >> status;
        stripCR(status); 

        bills.add(Bill(billId, patientId, appointmentId, amount, status)); 
    }

    file.close();
    return true;
}

// Load Prescriptions
bool FileHandler::loadPrescriptions(const char* filename, Storage<Prescription>& prescriptions) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    int prescriptionId, appointmentId, patientId, doctorId;
    char date[20], medicines[500], notes[300];

    while (file >> prescriptionId) {
        file.get();  
        file >> appointmentId;
        file.get();  
        file >> patientId;
        file.get(); 
        file >> doctorId;
        file.get(); 
        file >> date;
        file.get();  
        file.getline(medicines, 500, ',');
        file.getline(notes, 300, ','); // Skip the newline after notes

        prescriptions.add(Prescription(prescriptionId, appointmentId, patientId, doctorId, date, medicines, notes));  // Add Prescription directly
    }

    file.close();
    return true;
}

bool FileHandler::saveAdmins(const char* filename, const Storage<Admin>& admins) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    for (int i = 0; i < admins.size(); ++i) {
        if (admins.getAll()[i].getId() == 0) continue;
        file << admins.getAll()[i].getId() << ","
            << admins.getAll()[i].getName() << ","
            << admins.getAll()[i].getPassword() << "\n";
    }

    file.close();
    return true;
}

// Save Patients
bool FileHandler::savePatients(const char* filename, const Storage<Patient>& patients) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;

    for (int i = 0; i < patients.size(); ++i) {
        if (patients.getAll()[i].getId() == 0) continue;
        file << patients.getAll()[i].getId() << ","
            << patients.getAll()[i].getName() << ","
            << patients.getAll()[i].getAge() << ","
            << patients.getAll()[i].getGender() << ","
            << patients.getAll()[i].getContact() << ","
            << patients.getAll()[i].getPassword() << ","
            << patients.getAll()[i].getBalance() << "\n";
    }

    file.close();
    return true;
}

// Save Doctors
bool FileHandler::saveDoctors(const char* filename, const Storage<Doctor>& doctors) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    for (int i = 0; i < doctors.size(); ++i) {
        if (doctors.getAll()[i].getId() == 0) continue;
        file << doctors.getAll()[i].getId() << ","
            << doctors.getAll()[i].getName() << ","
            << doctors.getAll()[i].getPassword() << ","
            << doctors.getAll()[i].getSpecialization() << ","
            << doctors.getAll()[i].getFee() << "\n";
    }

    file.close();
    return true;
}

// Save Appointments
bool FileHandler::saveAppointments(const char* filename, const Storage<Appointment>& appointments) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    for (int i = 0; i < appointments.size(); ++i) {
        if (appointments.getAll()[i].getAppointmentId() == 0) continue;
        file << appointments.getAll()[i].getAppointmentId() << ","
            << appointments.getAll()[i].getPatientId() << ","
            << appointments.getAll()[i].getDoctorId() << ","
            << appointments.getAll()[i].getDate() << ","
            << appointments.getAll()[i].getTimeSlot() << ","
            << appointments.getAll()[i].getStatus() << "\n";
    }

    file.close();
    return true;
}

void FileHandler::archivePatientRecord(int patientId, Storage<Patient>& patients,
    Storage<Appointment>& appointments,
    Storage<Bill>& bills,
    const char* archiveFile) {
    std::ofstream outFile(archiveFile, std::ios::app);
    if (!outFile) {
        std::cerr << "Error opening archive file." << std::endl;
        return;
    }

    Patient* patient = nullptr;
    for (int i = 0; i < patients.size(); ++i) {
        if (patients.getAll()[i].getId() == patientId) {
            patient = &patients.getAll()[i];
            break;
        }
    }

    if (patient) {
        outFile << patient->getId() << ", " << patient->getName() << ", "
            << patient->getAge() << ", " << patient->getGender() << ", "
            << patient->getContact() << ", " << patient->getBalance() << std::endl;

        // Archive appointments
        for (int i = 0; i < appointments.size(); ++i) {
            Appointment& app = appointments.getAll()[i];
            if (app.getPatientId() == patientId) {
                outFile << app.getAppointmentId() << ", " << app.getDoctorId() << ", "
                    << app.getDate() << ", " << app.getTimeSlot() << ", "
                    << app.getStatus() << std::endl;
            }
        }

        // Archive bills
        for (int i = 0; i < bills.size(); ++i) {
            Bill& bill = bills.getAll()[i];
            if (bill.getPatientId() == patientId) {
                outFile << bill.getBillId() << ", " << bill.getAmount() << ", "
                    ", " << bill.getStatus() << std::endl;
            }
        }

        outFile.close();
    }
}

// Save Bills
bool FileHandler::saveBills(const char* filename, const Storage<Bill>& bills) {

    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    for (int i = 0; i < bills.size(); ++i) {
        if (bills.getAll()[i].getBillId() == 0) continue;
        file << bills.getAll()[i].getBillId() << ","
            << bills.getAll()[i].getPatientId() << ","
            << bills.getAll()[i].getAppointmentId() << ","
            << bills.getAll()[i].getAmount() << ","
            << bills.getAll()[i].getStatus() << "\n";
    }

    file.close();
    return true;
}

// Save Prescriptions
bool FileHandler::savePrescriptions(const char* filename, const Storage<Prescription>& prescriptions) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    for (int i = 0; i < prescriptions.size(); ++i) {
        if (prescriptions.getAll()[i].getPrescriptionId() == 0) continue;
        file << prescriptions.getAll()[i].getPrescriptionId() << ","
            << prescriptions.getAll()[i].getAppointmentId() << ","
            << prescriptions.getAll()[i].getPatientId() << ","
            << prescriptions.getAll()[i].getDoctorId() << ","
            << prescriptions.getAll()[i].getDate() << ","
            << prescriptions.getAll()[i].getMedicines() << ","
            << prescriptions.getAll()[i].getNotes() << "\n";
    }

    file.close();
    return true;
}