// database.cpp

#include <iostream>
#include "database/database.hpp"
#include "config/config.hpp"
#include "logger/logger.hpp"

Database::Database() : conn(nullptr) {}

Database::~Database() {
    if (conn!= nullptr) {
        delete conn;
        conn = nullptr;
        logger.logMessage("Database connection closed.");
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
            logger.logMessage(std::string("Connected to database: ") + conn->dbname());
        } else {
            throw std::runtime_error("Failed to open database connection");
        }

    } catch (const std::exception& e) {
        logger.logMessage("Error connecting to database: " + std::string(e.what()));
    }
}