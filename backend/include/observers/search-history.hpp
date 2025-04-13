#ifndef SEARCH_HISTORY_HPP
#define SEARCH_HISTORY_HPP

#include <unordered_map>
#include <list>
#include <string>
#include <mutex>
#include "iobserver.hpp"

class SearchHistory : public IObserver {
public:
    void update(const std::unordered_map<std::string, std::vector<std::string>>& filters,
                const std::unordered_set<int>& results) override;

    const std::unordered_map<std::string, std::list<std::string>>& getSuggestions() const;
    void printSuggestions() const;

private:
    static const size_t MAX_SUGGESTIONS = 50;
    std::unordered_map<std::string, std::list<std::string>> suggestions;
    std::mutex mutex;

};

#endif // SEARCH_HISTORY_HPP
