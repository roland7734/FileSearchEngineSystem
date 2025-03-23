// config.cpp

#include <iostream>
#include "config/config.hpp"

namespace Config {
    std::string getConfigSummary() {
        return "Database: " + std::string(DB_NAME) + "@" + DB_HOST +
               "\nRoot Directory: " + ROOT_DIRECTORY +
               "\nBatch Size: " + std::to_string(BATCH_SIZE)
    }

    void logConfig() {
            std::cout << "File Crawler Configuration:\n" << getConfigSummary() << std::endl;
    }
}
