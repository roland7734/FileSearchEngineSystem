#include <iostream>
#include "config/config.hpp"
#include "logger/logger-manager.hpp"
#include <nlohmann/json.hpp>

namespace Config {
    std::string getConfigSummary() {
        return "Database: " + std::string(DB_NAME) + "@" + DB_HOST +
               "\nBatch Size: " + std::to_string(BATCH_SIZE) + "\n";
    }

    void logConfig() {
            LoggerManager::instance().logMessage("File Crawler Configuration:\n" + getConfigSummary());
    }
    void loadConfig(const std::string& configFilePath) {
        std::ifstream file(configFilePath);
        if (!file.is_open()) {
            LoggerManager::instance().logMessage("Error: Unable to open config file: " + configFilePath);
            return;
        }

        try {
            nlohmann::json config;
            file >> config;

            if (config.contains("DB_HOST")) DB_HOST = config["DB_HOST"].get<std::string>();
            if (config.contains("DB_PORT")) DB_PORT = config["DB_PORT"].get<int>();
            if (config.contains("DB_NAME")) DB_NAME = config["DB_NAME"].get<std::string>();
            if (config.contains("DB_USER")) DB_USER = config["DB_USER"].get<std::string>();
            if (config.contains("DB_PASSWORD")) DB_PASSWORD = config["DB_PASSWORD"].get<std::string>();



        } catch (const std::exception& e) {
            LoggerManager::instance().logMessage("Error parsing config file: " + std::string(e.what()));
        }
    }

}
