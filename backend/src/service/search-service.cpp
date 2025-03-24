#include <pqxx/pqxx>
#include "service/search-service.hpp"
#include "database/database.hpp"
#include "model/file.hpp"
#include "logger/logger.hpp"
#include <iostream>
#include <sstream>
#include <vector>

SearchService::SearchService(Database *db) : db(db) {}

std::vector<File> SearchService::searchFileNames(const std::string &keyword) {
    std::vector <File> results;

    pqxx::connection *conn = db->getConnection();
    if (!conn || !conn->is_open()) {
        logger.logMessage("Database connection is not open during searchFileNames.");
        return results;
    }

    logger.logMessage("Executing file name search with keyword: " + keyword);
    auto start = std::chrono::high_resolution_clock::now();

    try {
        pqxx::work txn(*conn);

        std::string query = R"(SELECT path, ts_headline('simple', name, plainto_tsquery($1), 'MaxFragments=1, MaxWords=100'), LEFT(text_content, 100) AS preview
                          FROM files
                          WHERE to_tsvector('simple', name) @@ plainto_tsquery($1 || ':*')
                          LIMIT 20)";

        pqxx::result res = txn.exec_params(query, keyword);

        for (const auto &row: res) {
            std::string path = row[0].c_str();

            size_t lastSlashPos = path.find_last_of("/\\");
            std::string directory = (lastSlashPos != std::string::npos) ? path.substr(0, lastSlashPos + 1)
                                                                        : "";  // Keep the directory part
            size_t lastDotPos = path.find_last_of(".");

            std::string extension = (lastDotPos != std::string::npos) ? path.substr(lastDotPos + 1) : "";

            std::string newPath = directory + row[1].c_str() + "." + extension;
            results.emplace_back(newPath, row[2].c_str());
        }

        txn.commit();
    } catch (const pqxx::sql_error &e) {
        logger.logMessage("SQL error during searchFileNames: " + std::string(e.what()) +
                          " | Query: " + e.query() + " | Input Keyword: " + keyword);
        logger.logMessage("Search query for keyword: " + keyword + " has failed.");
        return results;
    } catch (const pqxx::broken_connection &e) {
        logger.logMessage("Database connection error: " + std::string(e.what()));
        logger.logMessage("Search query for keyword: " + keyword + " has failed.");
        return results;
    } catch (const std::exception &e) {
        logger.logMessage("Error during searchFileNames: " + std::string(e.what()));
        logger.logMessage("Search query for keyword: " + keyword + " has failed.");
        return results;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    logger.logUserSearchFileName(keyword, results.size());
    logger.logSearchPerformanceFileName(keyword, duration.count());
    return results;
}

std::vector<File> SearchService::searchTextContentBySingleWord(const std::string &text) {
    std::vector <File> results;

    pqxx::connection *conn = db->getConnection();
    if (!conn || !conn->is_open()) {
        logger.logMessage("Database connection is not open during searchTextContentBySingleWord.");
        return results;
    }

    logger.logMessage("Executing text content search with keyword: " + text);
    auto start = std::chrono::high_resolution_clock::now();
    try {
        pqxx::work txn(*conn);

        std::string query = R"(SELECT path, ts_headline('english', text_content, plainto_tsquery($1), 'MaxFragments=1, MaxWords=100') AS preview
                          FROM files
                          WHERE to_tsvector('english', text_content) @@ plainto_tsquery($1)
                          LIMIT 5)";

        pqxx::result res = txn.exec_params(query, text);

        for (const auto &row: res) {
            results.emplace_back(row[0].c_str(), row[1].c_str());
        }

        txn.commit();
    } catch (const pqxx::sql_error &e) {
        logger.logMessage("SQL error during searchTextContentBySingleWord: " + std::string(e.what()) +
                          " | Query: " + e.query() + " | Input Text: " + text);
        logger.logMessage("Search query for Input Text: \"" + text + "\" has failed.");
        return results;
    } catch (const pqxx::broken_connection &e) {
        logger.logMessage("Database connection error: " + std::string(e.what()));
        logger.logMessage("Search query for Input Text: \"" + text + "\" has failed.");
        return results;
    } catch (const std::exception &e) {
        logger.logMessage("Error during searchTextContentBySingleWord: " + std::string(e.what()));
        logger.logMessage("Search query for Input Text: \"" + text + "\" has failed.");
        return results;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    logger.logUserSearchTextContent(text, results.size());
    logger.logSearchPerformanceTextContent(text, duration.count());
    return results;
}

std::vector<File> SearchService::searchTextContentByMultipleWords(const std::string &text) {
    return searchTextContentBySingleWord(text);
}