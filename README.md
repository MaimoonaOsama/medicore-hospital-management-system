# MediCore Hospital Management System

Object Oriented Programming Project - Spring 2026

## Important
GitHub Repository Link: https://github.com/your-username/medicore-hospital-management-system

## Files Included
All required `.h` and `.cpp` files are included. Each main class has its own header and source file.

Required text files are also included:
- patients.txt
- doctors.txt
- admin.txt
- appointments.txt
- prescriptions.txt
- bills.txt
- security_log.txt
- discharged.txt

## Compile With SFML GUI
SFML is used for the first role-selection GUI window. After choosing a role, the full required hospital system continues in the console.

Linux command:

```bash
g++ -std=c++17 -DUSE_SFML *.cpp -o medicore -lsfml-graphics -lsfml-window -lsfml-system
```

Run:

```bash
./medicore
```

## Compile Without SFML For Console Testing Only
This is only for testing if SFML is not installed:

```bash
g++ -std=c++17 *.cpp -o medicore
./medicore
```

## Login Details For Testing

Admin:
- ID: 1
- Password: admin123

Patient:
- ID: 1
- Password: pass123

Doctor:
- ID: 1
- Password: doc456

## Notes
- No `std::vector` is used.
- Core hospital logic avoids `std::string`; `std::string` is only used inside the SFML GUI file where allowed.
- Data is loaded from and saved to text files.
- Missing required files cause startup failure through `FileNotFoundException`.
- Sorting is implemented manually.
- Custom exceptions are implemented.
- Operator overloading is implemented for Patient, Doctor and Appointment.
