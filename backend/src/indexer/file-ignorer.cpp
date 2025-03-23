// file-ignorer.cpp

#include <iostream>
#include "indexer/file-ignorer.hpp"

FileIgnorer::FileIgnorer(const std::vector<std::string>& patterns) {
    for (const auto& pattern : patterns) {
        ignore_patterns.push_back(std::regex(pattern));
    }
}

bool FileIgnorer::shouldIgnore(const std::string& filename) {
    for (const auto& pattern : ignore_patterns) {
        if (std::regex_search(filename, pattern)) {
            return true;
        }
    }
    return false;
}
