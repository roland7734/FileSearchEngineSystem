#ifndef USAGE_STATS_SERVICE_HPP
#define USAGE_STATS_SERVICE_HPP

#include "database/database.hpp"
#include <string>

class UsageStatsService {
public:
    explicit UsageStatsService(Database* db);

    bool incrementAccessCount(const std::string& filePath);
    bool incrementSearchCount(const std::string& filePath);

private:
    Database* db;

    bool incrementColumn(const std::string& filePath, const std::string& column);
};

#endif // USAGE_STATS_SERVICE_HPP
