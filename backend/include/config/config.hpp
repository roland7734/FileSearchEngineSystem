// config.hpp

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <iostream>


namespace Config {

    // Database Configuration
    inline std::string DB_HOST;
    inline int DB_PORT;
    inline std::string DB_NAME;
    inline std::string DB_USER;
    inline std::string DB_PASSWORD;

    // Crawler Settings
    inline constexpr int BATCH_SIZE = 100;
    inline constexpr int MAXIMUM_CONTENT_SIZE = 1048575;

    // Supported Text Extensions
    const std::unordered_set<std::string> TEXT_EXTENSIONS = {
            ".txt", ".md", ".cpp", ".py", ".h", ".hpp", ".java", ".js", ".html", ".css",
            ".json", ".xml", ".yaml", ".yml", ".sh", ".bat", ".csv", ".tsv", ".ini", ".log",
            ".sql", ".php", ".rb", ".swift", ".kt", ".rs", ".go", ".pl", ".r", ".lua",
            ".elm", ".hs"
    };

    std::string getConfigSummary();
    void logConfig();
    void loadConfig(const std::string& configFilePath);

}

#endif  // CONFIG_HPP
