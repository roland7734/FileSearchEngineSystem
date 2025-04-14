#ifndef TEST_UTILS_HPP
#define TEST_UTILS_HPP

#include <fstream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

inline void createTempFile(const std::string& path, const std::string& content) {
    fs::create_directories(fs::path(path).parent_path());
    std::ofstream out(path);
    out << content;
    out.close();
}

#endif  // TEST_UTILS_HPP
