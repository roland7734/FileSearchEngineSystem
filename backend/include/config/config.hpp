#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iostream>


namespace Config {

    inline std::string DB_HOST;
    inline int DB_PORT;
    inline std::string DB_NAME;
    inline std::string DB_USER;
    inline std::string DB_PASSWORD;
    inline const char MAGIC_DB_PATH[] = "D:\\Facultate\\3nd Year Sem2\\SD\\D\\installed\\x64-windows\\share\\libmagic\\misc\\magic.mgc";

    inline constexpr int BATCH_SIZE = 100;
    inline constexpr int MAXIMUM_CONTENT_SIZE = 1048575;

    inline constexpr double PATH_DEPTH_WEIGHT = 100.0;

    inline constexpr int SMALL_FILE_SIZE = 1024;
    inline constexpr double SMALL_FILE_BONUS = 5.0;
    inline constexpr double SEARCH_COUNT_SCORE = 10.0;
    inline constexpr double ACCESS_COUNT_SCORE = 10.0;
    inline constexpr double CREATED_TIME_DECAY_PER_DAY = 0.1;

    const std::unordered_map<std::string, double> DIRECTORY_BONUSES = {
            {"/src/", 20.0},
            {"/include/", 15.0},
            {"/docs/", 10.0},
            {"/test/", 5.0}
    };

    const std::unordered_set<std::string> TEXT_EXTENSIONS = {
            ".txt", ".md", ".cpp", ".py", ".h", ".hpp", ".java", ".js", ".html", ".css",
            ".json", ".xml", ".yaml", ".yml", ".sh", ".bat", ".csv", ".tsv", ".ini", ".log",
            ".sql", ".php", ".rb", ".swift", ".kt", ".rs", ".go", ".pl", ".r", ".lua",
            ".elm", ".hs"
    };

    const std::unordered_map<std::string, std::string> EXTENSION_TO_LANGUAGE = {
            {".cpp", "C++"}, {".h", "C/C++ Header"}, {".hpp", "C++"},
            {".py", "Python"}, {".java", "Java"}, {".js", "JavaScript"},
            {".html", "HTML"}, {".css", "CSS"}, {".json", "JSON"},
            {".xml", "XML"}, {".yaml", "YAML"}, {".yml", "YAML"},
            {".sh", "Shell"}, {".bat", "Batch"}, {".sql", "SQL"},
            {".php", "PHP"}, {".rb", "Ruby"}, {".swift", "Swift"},
            {".kt", "Kotlin"}, {".rs", "Rust"}, {".go", "Go"},
            {".pl", "Perl"}, {".r", "R"}, {".lua", "Lua"},
            {".elm", "Elm"}, {".hs", "Haskell"}, {".md", "Markdown"},
            {".ini", "INI"}, {".csv", "CSV"}, {".tsv", "TSV"},
            {".log", "Log"}
    };


    std::string getConfigSummary();
    void loadConfig(const std::string& configFilePath);

}

#endif  // CONFIG_HPP
