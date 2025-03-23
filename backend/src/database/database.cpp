// database.cpp

#include <iostream>
#include "database/database.hpp"
#include "config/config.hpp"

Database::Database() : conn(nullptr) {}

Database::~Database() {
    if (conn!= nullptr) {
        delete conn;
        conn = nullptr;
    }
}

pqxx::connection *Database::getConnection() {
    return conn;
}

void Database::connect() {
    try {
        std::string connection_string = "host=" + std::string(Config::DB_HOST) +
                                        " port=" + std::to_string(Config::DB_PORT) +
                                        " dbname=" + std::string(Config::DB_NAME) +
                                        " user=" + std::string(Config::DB_USER) +
                                        " password=" + std::string(Config::DB_PASSWORD);

        conn = new pqxx::connection(connection_string);

        if (conn->is_open()) {
            std::cout << "Connected to database: " << conn->dbname() << std::endl;
        } else {
            throw std::runtime_error("Failed to open database connection");
        }

    } catch (const std::exception& e) {
        std::cerr << "Error connecting to database: " << e.what() << std::endl;
    }
}