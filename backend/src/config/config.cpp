// config.cpp

#include <iostream>
#include "config/config.hpp"
#include "logger/logger.hpp"

namespace Config {
    std::string getConfigSummary() {
        return "Database: " + std::string(DB_NAME) + "@" + DB_HOST +
               "\nBatch Size: " + std::to_string(BATCH_SIZE) + "\n";
    }

    void logConfig() {
            logger.logMessage("File Crawler Configuration:\n" + getConfigSummary());
    }
}
