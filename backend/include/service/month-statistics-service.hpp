#ifndef MONTH_STATISTICS_SERVICE_HPP
#define MONTH_STATISTICS_SERVICE_HPP

#include <nlohmann/json.hpp>
#include "database/database.hpp"

class MonthStatisticsService {
public:
    explicit MonthStatisticsService(Database* db);
    nlohmann::json getFileCountsByMonthForYear(int year);

private:
    Database* db;
};

#endif
