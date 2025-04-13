#include "observers/search-history.hpp"
#include <iostream>

void SearchHistory::update(const std::unordered_map<std::string, std::vector<std::string>>& filters,
                           const std::unordered_set<int>& results) {
    std::lock_guard<std::mutex> lock(mutex);

    for (const auto& [key, values] : filters) {
        auto& list = suggestions[key];

        for (const auto& value : values) {
            list.remove(value);
            list.push_front(value);
        }

        while (list.size() > MAX_SUGGESTIONS) {
            list.pop_back();
        }
    }

    printSuggestions();
}

const std::unordered_map<std::string, std::list<std::string>>& SearchHistory::getSuggestions() const {
    return suggestions;
}

void SearchHistory::printSuggestions() const {
    std::cout << "\n==== Search Suggestions ====\n";
    for (const auto& [key, values] : suggestions) {
        std::cout << "[" << key << "]: ";
        int count = 0;
        for (const auto& suggestion : values) {
            std::cout << suggestion;
            if (++count < values.size()) std::cout << ", ";
        }
        std::cout << "\n";
    }
    std::cout << "============================\n";
}