// config.hpp

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

namespace Config {

    // Database Configuration
    inline constexpr char DB_HOST[] = "localhost";
    inline constexpr int DB_PORT = 5432;
    inline constexpr char DB_NAME[] = "file_system";
    inline constexpr char DB_USER[] = "postgres";
    inline constexpr char DB_PASSWORD[] = "postgreSQL";

    // Crawler Settings
    inline constexpr int BATCH_SIZE = 100;

    std::string getConfigSummary();
    void logConfig();
}

#endif  // CONFIG_HPP
