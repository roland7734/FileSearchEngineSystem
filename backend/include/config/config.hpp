// config.hpp

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <unordered_set>

namespace Config {

    // Database Configuration
    inline constexpr char DB_HOST[] = "localhost";
    inline constexpr int DB_PORT = 5432;
    inline constexpr char DB_NAME[] = "file_system";
    inline constexpr char DB_USER[] = "postgres";
    inline constexpr char DB_PASSWORD[] = "postgreSQL";

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
}

#endif  // CONFIG_HPP
