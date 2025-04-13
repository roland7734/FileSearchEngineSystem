#include <iostream>
#include "indexer/file-ignorer.hpp"

FileIgnorer::FileIgnorer(const std::vector<std::string>& patterns) : ignore_patterns(patterns) {}

bool FileIgnorer::shouldIgnore(const std::string& filename) {
    for (const auto& pattern : ignore_patterns) {
        if (filename.find(pattern) != std::string::npos) {
            return true;
        }
    }
    return false;
}
