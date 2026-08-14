#include <iostream>
#include <string>
#include <fstream>

#include "database.h"

using namespace std;

// ==================== Function Declarations ====================

// Patient Management
void registerPatient(Database& database);
void loginPatient(Database& database);
void viewPatients(Database& database);

// Doctor Management
void addDoctor(Database& database);
void viewDoctors(Database& database);

// Appointment Management
void bookAppointment(Database& database);
void viewAppointments(Database& database);

// Medical Records
void addMedicalRecord(Database& database);
void viewMedicalRecords(Database& database);

// Medical Reports
void uploadMedicalReport(Database& database);
void viewMedicalReport();

// User Management
void createUser(Database& database);

// Admin
void adminPanel(Database& database);


// ==================== Main Function ====================

int main() {

    Database database;

    // Open database
    if (!database.open()) {
        return 1;
    }

    // Initialize tables
    if (!database.initialize()) {
        return 1;
    }

    int choice;

    while (true) {

        cout << "\n";
        cout << "============================================\n";
        cout << "        HOSPITAL MANAGEMENT SYSTEM\n";
        cout << "============================================\n";

        cout << "1.  Register Patient\n";
        cout << "2.  Patient Login\n";
        cout << "3.  View Patients\n";
        cout << "4.  Add Doctor\n";
        cout << "5.  View Doctors\n";
        cout << "6.  Book Appointment\n";
        cout << "7.  View Appointments\n";
        cout << "8.  View Medical Records\n";
        cout << "9.  Add Medical Record\n";
        cout << "10. Upload Medical Report\n";
        cout << "11. View Medical Report\n";
        cout << "12. Create User\n";
        cout << "13. Admin Panel\n";
        cout << "14. Exit\n";

        cout << "\nEnter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {

            case 1:
                registerPatient(database);
                break;

            case 2:
                loginPatient(database);
                break;

            case 3:
                viewPatients(database);
                break;

            case 4:
                addDoctor(database);
                break;

            case 5:
                viewDoctors(database);
                break;

            case 6:
                bookAppointment(database);
                break;

            case 7:
                viewAppointments(database);
                break;

            case 8:
                viewMedicalRecords(database);
                break;

            case 9:
                addMedicalRecord(database);
                break;

            case 10:
                uploadMedicalReport(database);
                break;

            case 11:
                viewMedicalReport();
                break;

            case 12:
                createUser(database);
                break;

            case 13:
                adminPanel(database);
                break;

            case 14:
                cout << "\nExiting application...\n";
                return 0;

            default:
                cout << "\nInvalid choice! Please try again.\n";
        }
    }

    return 0;
}


// ============================================================
// 1. PATIENT REGISTRATION
// ============================================================

void registerPatient(Database& database) {

    string name;
    string phone;
    string username;
    string password;
    int age;

    cout << "\n========== PATIENT REGISTRATION ==========\n";

    cout << "Enter name: ";
    getline(cin, name);

    cout << "Enter age: ";
    cin >> age;
    cin.ignore();

    cout << "Enter phone: ";
    getline(cin, phone);

    cout << "Enter username: ";
    getline(cin, username);

    cout << "Enter password: ";
    getline(cin, password);

    // INTENTIONALLY VULNERABLE:
    // User input is directly concatenated into SQL.
    string sql =
        "INSERT INTO patients "
        "(name, age, phone, username, password, role) "
        "VALUES ('" +
        name + "', " +
        to_string(age) + ", '" +
        phone + "', '" +
        username + "', '" +
        password + "', 'PATIENT');";

    char* errorMessage = nullptr;

    int result = sqlite3_exec(
        database.getConnection(),
        sql.c_str(),
        nullptr,
        nullptr,
        &errorMessage
    );

    if (result != SQLITE_OK) {

        cout << "\nRegistration failed!\n";

        if (errorMessage != nullptr) {
            cout << "Database error: "
                 << errorMessage << endl;

            sqlite3_free(errorMessage);
        }

    } else {

        cout << "\nPatient registered successfully!\n";
    }
}


// ============================================================
// 2. PATIENT LOGIN
// ============================================================

void loginPatient(Database& database) {

    string username;
    string password;

    cout << "\n========== PATIENT LOGIN ==========\n";

    cout << "Username: ";
    getline(cin, username);

    cout << "Password: ";
    getline(cin, password);

    // ========================================================
    // VULNERABILITY #1: SQL INJECTION
    // ========================================================
    //
    // User input is directly inserted into the SQL query.
    // Prepared statements are intentionally NOT used.
    //
    // ========================================================

    string sql =
        "SELECT id, name, role FROM patients "
        "WHERE username='" +
        username +
        "' AND password='" +
        password +
        "';";

    sqlite3_stmt* statement;

    int result = sqlite3_prepare_v2(
        database.getConnection(),
        sql.c_str(),
        -1,
        &statement,
        nullptr
    );

    if (result != SQLITE_OK) {

        cout << "Login query failed.\n";
        return;
    }

    if (sqlite3_step(statement) == SQLITE_ROW) {

        int id =
            sqlite3_column_int(statement, 0);

        string name =
            reinterpret_cast<const char*>(
                sqlite3_column_text(statement, 1)
            );

        string role =
            reinterpret_cast<const char*>(
                sqlite3_column_text(statement, 2)
            );

        cout << "\nLogin successful!\n";
        cout << "Patient ID: " << id << endl;
        cout << "Name: " << name << endl;
        cout << "Role: " << role << endl;

    } else {

        cout << "\nInvalid username or password.\n";
    }

    sqlite3_finalize(statement);
}


// ============================================================
// 3. VIEW PATIENTS
// ============================================================

void viewPatients(Database& database) {

    cout << "\n========== PATIENT LIST ==========\n";

    const char* sql =
        "SELECT id, name, age, phone, username, role "
        "FROM patients;";

    sqlite3_stmt* statement;

    int result = sqlite3_prepare_v2(
        database.getConnection(),
        sql,
        -1,
        &statement,
        nullptr
    );

    if (result != SQLITE_OK) {

        cout << "Unable to retrieve patients.\n";
        return;
    }

    bool found = false;

    while (sqlite3_step(statement) == SQLITE_ROW) {

        found = true;

        int id =
            sqlite3_column_int(statement, 0);

        string name =
            reinterpret_cast<const char*>(
                sqlite3_column_text(statement, 1)
            );

        int age =
            sqlite3_column_int(statement, 2);

        string phone =
            reinterpret_cast<const char*>(
                sqlite3_column_text(statement, 3)
            );

        string username =
            reinterpret_cast<const char*>(
                sqlite3_column_text(statement, 4)
            );

        string role =
            reinterpret_cast<const char*>(
                sqlite3_column_text(statement, 5)
            );

        cout << "\n-----------------------------\n";
        cout << "ID: " << id << endl;
        cout << "Name: " << name << endl;
        cout << "Age: " << age << endl;
        cout << "Phone: " << phone << endl;
        cout << "Username: " << username << endl;
        cout << "Role: " << role << endl;
    }

    if (!found) {
        cout << "No patients found.\n";
    }

    sqlite3_finalize(statement);
}


// ============================================================
// 4. ADD DOCTOR
// ============================================================

void addDoctor(Database& database) {

    string name;
    string specialization;
    string phone;
    string email;

    cout << "\n========== ADD DOCTOR ==========\n";

    cout << "Doctor name: ";
    getline(cin, name);

    cout << "Specialization: ";
    getline(cin, specialization);

    cout << "Phone: ";
    getline(cin, phone);

    cout << "Email: ";
    getline(cin, email);

    string sql =
        "INSERT INTO doctors "
        "(name, specialization, phone, email) "
        "VALUES ('" +
        name + "', '" +
        specialization + "', '" +
        phone + "', '" +
        email + "');";

    char* errorMessage = nullptr;

    int result = sqlite3_exec(
        database.getConnection(),
        sql.c_str(),
        nullptr,
        nullptr,
        &errorMessage
    );

    if (result != SQLITE_OK) {

        cout << "Failed to add doctor.\n";

        if (errorMessage != nullptr) {
            cout << "Database error: "
                 << errorMessage << endl;

            sqlite3_free(errorMessage);
        }

    } else {

        cout << "\nDoctor added successfully!\n";
    }
}


// ============================================================
// 5. VIEW DOCTORS
// ============================================================

void viewDoctors(Database& database) {

    cout << "\n========== DOCTOR LIST ==========\n";

    const char* sql =
        "SELECT id, name, specialization, phone, email "
        "FROM doctors;";

    sqlite3_stmt* statement;

    int result = sqlite3_prepare_v2(
        database.getConnection(),
        sql,
        -1,
        &statement,
        nullptr
    );

    if (result != SQLITE_OK) {

        cout << "Unable to retrieve doctors.\n";
        return;
    }

    bool found = false;

    while (sqlite3_step(statement) == SQLITE_ROW) {

        found = true;

        int id =
            sqlite3_column_int(statement, 0);

        string name =
            reinterpret_cast<const char*>(
                sqlite3_column_text(statement, 1)
            );

        string specialization =
            reinterpret_cast<const char*>(
                sqlite3_column_text(statement, 2)
            );

        string phone =
            reinterpret_cast<const char*>(
                sqlite3_column_text(statement, 3)
            );

        string email =
            reinterpret_cast<const char*>(
                sqlite3_column_text(statement, 4)
            );

        cout << "\n-----------------------------\n";
        cout << "Doctor ID: " << id << endl;
        cout << "Name: " << name << endl;
        cout << "Specialization: "
             << specialization << endl;
        cout << "Phone: " << phone << endl;
        cout << "Email: " << email << endl;
    }

    if (!found) {
        cout << "No doctors found.\n";
    }

    sqlite3_finalize(statement);
}


// ============================================================
// 6. BOOK APPOINTMENT
// ============================================================

void bookAppointment(Database& database) {

    int patientId;
    int doctorId;

    string appointmentDate;
    string reason;

    cout << "\n========== BOOK APPOINTMENT ==========\n";

    cout << "Patient ID: ";
    cin >> patientId;
    cin.ignore();

    cout << "Doctor ID: ";
    cin >> doctorId;
    cin.ignore();

    cout << "Appointment date: ";
    getline(cin, appointmentDate);

    cout << "Reason for appointment: ";
    getline(cin, reason);

    // Intentionally vulnerable SQL construction.

    string sql =
        "INSERT INTO appointments "
        "(patient_id, doctor_id, appointment_date, reason) "
        "VALUES (" +
        to_string(patientId) +
        ", " +
        to_string(doctorId) +
        ", '" +
        appointmentDate +
        "', '" +
        reason +
        "');";

    char* errorMessage = nullptr;

    int result = sqlite3_exec(
        database.getConnection(),
        sql.c_str(),
        nullptr,
        nullptr,
        &errorMessage
    );

    if (result != SQLITE_OK) {

        cout << "\nAppointment booking failed.\n";

        if (errorMessage != nullptr) {
            cout << "Database error: "
                 << errorMessage << endl;

            sqlite3_free(errorMessage);
        }

    } else {

        cout << "\nAppointment booked successfully!\n";
    }
}


// ============================================================
// 7. VIEW APPOINTMENTS
// ============================================================

void viewAppointments(Database& database) {

    cout << "\n========== APPOINTMENT LIST ==========\n";

    const char* sql = R"(
        SELECT
            appointments.id,
            patients.name,
            doctors.name,
            doctors.specialization,
            appointments.appointment_date,
            appointments.reason
        FROM appointments
        JOIN patients
            ON appointments.patient_id = patients.id
        JOIN doctors
            ON appointments.doctor_id = doctors.id;
    )";

    sqlite3_stmt* statement;

    int result = sqlite3_prepare_v2(
        database.getConnection(),
        sql,
        -1,
        &statement,
        nullptr
    );

    if (result != SQLITE_OK) {

        cout << "Unable to retrieve appointments.\n";
        return;
    }

    bool found = false;

    while (sqlite3_step(statement) == SQLITE_ROW) {

        found = true;

        int appointmentId =
            sqlite3_column_int(statement, 0);

        string patient =
            reinterpret_cast<const char*>(
                sqlite3_column_text(statement, 1)
            );

        string doctor =
            reinterpret_cast<const char*>(
                sqlite3_column_text(statement, 2)
            );

        string specialization =
            reinterpret_cast<const char*>(
                sqlite3_column_text(statement, 3)
            );

        string date =
            reinterpret_cast<const char*>(
                sqlite3_column_text(statement, 4)
            );

        string reason =
            reinterpret_cast<const char*>(
                sqlite3_column_text(statement, 5)
            );

        cout << "\n-----------------------------\n";
        cout << "Appointment ID: "
             << appointmentId << endl;

        cout << "Patient: "
             << patient << endl;

        cout << "Doctor: "
             << doctor << endl;

        cout << "Specialization: "
             << specialization << endl;

        cout << "Date: "
             << date << endl;

        cout << "Reason: "
             << reason << endl;
    }

    if (!found) {
        cout << "No appointments found.\n";
    }

    sqlite3_finalize(statement);
}


// ============================================================
// 8. VIEW MEDICAL RECORDS
// ============================================================

void viewMedicalRecords(Database& database) {

    int patientId;

    cout << "\n========== MEDICAL RECORDS ==========\n";

    cout << "Enter Patient ID: ";
    cin >> patientId;
    cin.ignore();

    // ========================================================
    // VULNERABILITY #2: BROKEN ACCESS CONTROL
    // ========================================================
    //
    // The application accepts any patient ID.
    //
    // It does NOT verify that the requested medical record
    // belongs to the currently authenticated patient.
    //
    // ========================================================

    string sql =
        "SELECT id, diagnosis, prescription "
        "FROM medical_records "
        "WHERE patient_id=" +
        to_string(patientId) +
        ";";

    sqlite3_stmt* statement;

    int result = sqlite3_prepare_v2(
        database.getConnection(),
        sql.c_str(),
        -1,
        &statement,
        nullptr
    );

    if (result != SQLITE_OK) {

        cout << "Unable to access medical records.\n";
        return;
    }

    bool found = false;

    while (sqlite3_step(statement) == SQLITE_ROW) {

        found = true;

        int id =
            sqlite3_column_int(statement, 0);

        string diagnosis =
            reinterpret_cast<const char*>(
                sqlite3_column_text(statement, 1)
            );

        string prescription =
            reinterpret_cast<const char*>(
                sqlite3_column_text(statement, 2)
            );

        cout << "\n-------------------------\n";
        cout << "Record ID: " << id << endl;
        cout << "Diagnosis: " << diagnosis << endl;
        cout << "Prescription: "
             << prescription << endl;
    }

    if (!found) {
        cout << "No medical records found.\n";
    }

    sqlite3_finalize(statement);
}


// ============================================================
// 9. ADD MEDICAL RECORD
// ============================================================

void addMedicalRecord(Database& database) {

    int patientId;
    string diagnosis;
    string prescription;

    cout << "\n========== ADD MEDICAL RECORD ==========\n";

    cout << "Patient ID: ";
    cin >> patientId;
    cin.ignore();

    cout << "Diagnosis: ";
    getline(cin, diagnosis);

    cout << "Prescription: ";
    getline(cin, prescription);

    // Intentionally vulnerable SQL construction.

    string sql =
        "INSERT INTO medical_records "
        "(patient_id, diagnosis, prescription) "
        "VALUES (" +
        to_string(patientId) +
        ", '" +
        diagnosis +
        "', '" +
        prescription +
        "');";

    char* errorMessage = nullptr;

    int result = sqlite3_exec(
        database.getConnection(),
        sql.c_str(),
        nullptr,
        nullptr,
        &errorMessage
    );

    if (result != SQLITE_OK) {

        cout << "Failed to add medical record.\n";

        if (errorMessage != nullptr) {
            cout << "Database error: "
                 << errorMessage << endl;

            sqlite3_free(errorMessage);
        }

    } else {

        cout << "Medical record added successfully!\n";
    }
}


// ============================================================
// 10. UPLOAD MEDICAL REPORT
// ============================================================

void uploadMedicalReport(Database& database) {

    int patientId;
    string filename;
    string sourcePath;

    cout << "\n========== UPLOAD MEDICAL REPORT ==========\n";

    cout << "Patient ID: ";
    cin >> patientId;
    cin.ignore();

    cout << "Enter source file path: ";
    getline(cin, sourcePath);

    cout << "Enter filename to store: ";
    getline(cin, filename);

    ifstream source(sourcePath, ios::binary);

    if (!source) {

        cout << "Unable to open source file.\n";
        return;
    }

    // ========================================================
    // VULNERABILITY #3: FILE UPLOAD VULNERABILITY
    // ========================================================
    //
    // No extension validation
    // No MIME/type validation
    // No file size validation
    // No filename sanitization
    //
    // ========================================================

    string destination =
        "secure_application/outputs/uploads/" +
        filename;

    ofstream destinationFile(
        destination,
        ios::binary
    );

    if (!destinationFile) {

        cout << "Unable to create destination file.\n";
        return;
    }

    destinationFile << source.rdbuf();

    source.close();
    destinationFile.close();

    cout << "\nMedical report uploaded successfully.\n";
    cout << "Stored as: "
         << destination << endl;
}


// ============================================================
// 11. VIEW MEDICAL REPORT
// ============================================================

void viewMedicalReport() {

    string filename;

    cout << "\n========== VIEW MEDICAL REPORT ==========\n";

    cout << "Enter report filename: ";
    getline(cin, filename);

    // ========================================================
    // VULNERABILITY #5: PATH TRAVERSAL
    // ========================================================
    //
    // The filename comes directly from the user.
    // No canonicalization or directory restriction is applied.
    //
    // ========================================================

    string path =
        "secure_application/outputs/uploads/" +
        filename;

    ifstream file(path);

    if (!file) {

        cout << "Unable to open medical report.\n";
        return;
    }

    cout << "\n========== REPORT CONTENT ==========\n";

    string line;

    while (getline(file, line)) {
        cout << line << endl;
    }

    file.close();
}


// ============================================================
// 12. CREATE USER
// ============================================================

void createUser(Database& database) {

    string name;
    string phone;
    string username;
    string password;
    string role;

    int age;

    cout << "\n========== CREATE USER ==========\n";

    cout << "Name: ";
    getline(cin, name);

    cout << "Age: ";
    cin >> age;
    cin.ignore();

    cout << "Phone: ";
    getline(cin, phone);

    cout << "Username: ";
    getline(cin, username);

    cout << "Password: ";
    getline(cin, password);

    cout << "Role (PATIENT/DOCTOR/ADMIN): ";
    getline(cin, role);

    // Intentionally vulnerable SQL construction.

    string sql =
        "INSERT INTO patients "
        "(name, age, phone, username, password, role) "
        "VALUES ('" +
        name +
        "', " +
        to_string(age) +
        ", '" +
        phone +
        "', '" +
        username +
        "', '" +
        password +
        "', '" +
        role +
        "');";

    char* errorMessage = nullptr;

    int result = sqlite3_exec(
        database.getConnection(),
        sql.c_str(),
        nullptr,
        nullptr,
        &errorMessage
    );

    if (result != SQLITE_OK) {

        cout << "User creation failed.\n";

        if (errorMessage != nullptr) {

            cout << "Database error: "
                 << errorMessage << endl;

            sqlite3_free(errorMessage);
        }

    } else {

        cout << "User created successfully.\n";
    }
}


// ============================================================
// 13. ADMIN PANEL
// ============================================================

void adminPanel(Database& database) {

    int choice;

    cout << "\n========== ADMIN PANEL ==========\n";

    // ========================================================
    // VULNERABILITY #4: MISSING AUTHORIZATION
    // ========================================================
    //
    // There is NO check that the current user has ADMIN role.
    //
    // Any user who reaches this menu can access administrative
    // functionality.
    //
    // ========================================================

    cout << "1. View All Patients\n";
    cout << "2. View All Doctors\n";
    cout << "3. View All Appointments\n";
    cout << "4. Exit Admin Panel\n";

    cout << "\nEnter choice: ";
    cin >> choice;
    cin.ignore();

    switch (choice) {

        case 1:
            viewPatients(database);
            break;

        case 2:
            viewDoctors(database);
            break;

        case 3:
            viewAppointments(database);
            break;

        case 4:
            cout << "Leaving admin panel.\n";
            break;

        default:
            cout << "Invalid choice.\n";
    }
}
