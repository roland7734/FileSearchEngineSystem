#include "service/usage-stats-service.hpp"
#include "logger/logger-manager.hpp"
#include <pqxx/pqxx>
#include <iostream>

UsageStatsService::UsageStatsService(Database* db) : db(db) {}

bool UsageStatsService::incrementAccessCount(const std::string& filePath) {
    return incrementColumn(filePath, "access_count");
}

bool UsageStatsService::incrementSearchCount(const std::string& filePath) {
    return incrementColumn(filePath, "search_count");
}

bool UsageStatsService::incrementColumn(const std::string& filePath, const std::string& column) {
    try {
        pqxx::connection* conn = db->getConnection();
        pqxx::work txn(*conn);

        txn.exec_params(
                "INSERT INTO file_usage_stats (file_id, " + column + ") "
                                                                     "SELECT id, 1 FROM files WHERE path = $1 "
                                                                     "ON CONFLICT (file_id) "
                                                                     "DO UPDATE SET " + column + " = file_usage_stats." + column + " + 1",
                filePath
        );

        txn.commit();
        LoggerManager::instance().logMessage("Successfully incremented " + column + " for file: " + filePath);
        return true;
    } catch (const std::exception& e) {
        LoggerManager::instance().logMessage("Error incrementing " + column + ": " + std::string(e.what()));
        return false;
    }
}
