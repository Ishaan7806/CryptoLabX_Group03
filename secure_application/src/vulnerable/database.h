#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <string>

class Database {
private:
    sqlite3* db;

public:
    Database();
    ~Database();

    bool open();
    void close();
    bool initialize();

    sqlite3* getConnection();
};

#endif
