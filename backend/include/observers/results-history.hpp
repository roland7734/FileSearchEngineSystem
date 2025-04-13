#ifndef RESULTS_HISTORY_HPP
#define RESULTS_HISTORY_HPP

#include "service/usage-stats-service.hpp"
#include "iobserver.hpp"
#include <unordered_set>
#include <mutex>

class ResultsHistory : public IObserver {
public:
    explicit ResultsHistory(Database* db);

    void update(const std::unordered_map<std::string, std::vector<std::string>>& filters,
                const std::unordered_set<int>& results) override;

private:
    Database* db;
    std::mutex mutex;
};

#endif // RESULTS_HISTORY_HPP