// database.hpp

#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <string>
#include <pqxx/pqxx>

class Database {
public:
    Database();
    ~Database();

    void connect();
    pqxx::connection* getConnection();

private:
    pqxx::connection* conn;
};

#endif // DATABASE_HPP