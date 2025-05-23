#include "service/month-statistics-service.hpp"
#include <pqxx/pqxx>

MonthStatisticsService::MonthStatisticsService(Database* db) : db(db) {}

nlohmann::json MonthStatisticsService::getFileCountsByMonthForYear(int year) {
    nlohmann::json resultJson;
    try {
        pqxx::connection* conn = db->getConnection();
        pqxx::work txn(*conn);

        std::string query = R"(
            SELECT TO_CHAR(created_at, 'MM') AS month, COUNT(*)
            FROM file_metadata
            WHERE EXTRACT(YEAR FROM created_at) = $1
            GROUP BY month
            ORDER BY month
        )";

        pqxx::result result = txn.exec_params(query, year);

        for (const auto& row : result) {
            std::string month = row[0].c_str();
            int count = row[1].as<int>();
            resultJson[month] = count;
        }

    } catch (const std::exception& e) {
        resultJson["error"] = "Failed to fetch monthly counts.";
        resultJson["details"] = e.what();
    }

    return resultJson;
}
