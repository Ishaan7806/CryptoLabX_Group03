#include "database.h"
#include <iostream>

Database::Database() {
    db = nullptr;
}

Database::~Database() {
    close();
}

bool Database::open() {
    int result = sqlite3_open("hospital.db", &db);

    if (result != SQLITE_OK) {
        std::cerr << "Database opening failed: "
                  << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    std::cout << "Database connected successfully.\n";
    return true;
}

void Database::close() {
    if (db != nullptr) {
        sqlite3_close(db);
        db = nullptr;
    }
}

sqlite3* Database::getConnection() {
    return db;
}

bool Database::initialize() {

    const char* sql = R"(
    CREATE TABLE IF NOT EXISTS patients (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        name TEXT NOT NULL,
        age INTEGER NOT NULL,
        phone TEXT NOT NULL,
        username TEXT UNIQUE NOT NULL,
        password TEXT NOT NULL,
        role TEXT NOT NULL DEFAULT 'PATIENT'
    );

    CREATE TABLE IF NOT EXISTS doctors (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        name TEXT NOT NULL,
        specialization TEXT NOT NULL,
        phone TEXT NOT NULL,
        email TEXT NOT NULL
    );

    CREATE TABLE IF NOT EXISTS appointments (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        patient_id INTEGER NOT NULL,
        doctor_id INTEGER NOT NULL,
        appointment_date TEXT NOT NULL,
        reason TEXT NOT NULL
    );

    CREATE TABLE IF NOT EXISTS medical_records (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        patient_id INTEGER NOT NULL,
        diagnosis TEXT NOT NULL,
        prescription TEXT NOT NULL
    );
)";

    char* errorMessage = nullptr;

    int result = sqlite3_exec(
        db,
        sql,
        nullptr,
        nullptr,
        &errorMessage
    );

    if (result != SQLITE_OK) {
        std::cerr << "Database initialization failed: "
                  << errorMessage << std::endl;

        sqlite3_free(errorMessage);
        return false;
    }

    return true;
}
