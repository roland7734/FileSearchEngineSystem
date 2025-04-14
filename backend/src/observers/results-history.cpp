#include "observers/results-history.hpp"
#include "logger/logger-manager.hpp"
#include <pqxx/pqxx>
#include <iostream>

ResultsHistory::ResultsHistory(Database* db) : db(db) {}

void ResultsHistory::update(const std::unordered_map<std::string, std::vector<std::string>>& filters,
                            const std::unordered_set<int>& results) {
    std::lock_guard<std::mutex> lock(mutex);

    if (results.empty()) return;

    try {
        pqxx::connection* conn = db->getConnection();
        pqxx::work txn(*conn);

        std::string query = "INSERT INTO file_usage_stats (file_id, search_count) VALUES ";
        bool first = true;
        for (int id : results) {
            if (!first) query += ", ";
            query += "(" + txn.quote(id) + ", 1)";
            first = false;
        }

        query += " ON CONFLICT (file_id) DO UPDATE SET search_count = file_usage_stats.search_count + 1;";
        std::cout << query << "\n";

        txn.exec(query);
        txn.commit();
    } catch (const std::exception& e) {
        LoggerManager::instance().logMessage("ResultsHistory failed to update search count: " + std::string(e.what()));
    }
}
