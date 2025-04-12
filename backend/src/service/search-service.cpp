#include <pqxx/pqxx>
#include "service/search-service.hpp"
#include "database/database.hpp"
#include "model/file.hpp"
#include "logger/logger.hpp"
#include "filters/IFilter.hpp"
#include "filters/PathNameFilter.hpp"
#include "filters/ContentFilter.hpp"
#include "utils/string-processor.hpp"
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

std::vector<File> SearchService::searchQuery(const std::vector<std::unique_ptr<IFilter>>& filters) {
    std::vector <File> results;

    pqxx::connection *conn = db->getConnection();
    if (!conn || !conn->is_open()) {
        logger.logMessage("Database connection is not open during searchQuery.");
        return results;
    }

    auto start = std::chrono::high_resolution_clock::now();
    std::string filters_query;

    try {
        pqxx::work txn(*conn);

        std::string base_query = "SELECT path, LEFT(text_content, 100) FROM files";
        std::vector<std::string> where_clauses;

        for (const auto& filter : filters) {

            if (const auto* contentFilter = dynamic_cast<const ContentFilter*>(filter.get())) {
                filters_query += "content:" + contentFilter->getKeyword() + " ";
                where_clauses.push_back("to_tsvector('english', text_content) @@ plainto_tsquery(" + txn.quote(contentFilter->getKeyword()) + ")");
            } else if (const auto* nameFilter = dynamic_cast<const PathNameFilter*>(filter.get())) {
                filters_query += "path:" + nameFilter->getKeyword() + " ";
                where_clauses.push_back("path ILIKE " + txn.quote(StringProcessor::escapeBackslash(nameFilter->getKeyword())+"%"));
            }

        }
        if (!where_clauses.empty()) {
            base_query += " WHERE ";
            for (size_t i = 0; i < where_clauses.size(); ++i) {
                base_query += where_clauses[i];
                if (i < where_clauses.size() - 1) {
                    base_query += " AND ";
                }
            }
        }

        base_query += " ORDER BY score DESC LIMIT 5;";

        std::cout<<"\n"<<base_query<<"\n";
        pqxx::result result = txn.exec(base_query);
        txn.commit();
        for (const auto &row: result) {
            results.emplace_back(row[0].c_str(), row[1].c_str());
        }


    } catch (const pqxx::sql_error &e) {
        logger.logMessage("SQL error during searchQuery: " + std::string(e.what()) +
                          " | Query: " + e.query() + " | Input Query: " + filters_query);
        logger.logMessage("Search query for Input Query: \"" + filters_query + "\" has failed.");
        return results;
    } catch (const pqxx::broken_connection &e) {
        logger.logMessage("Database connection error: " + std::string(e.what()));
        logger.logMessage("Search query for Input Query: \"" + filters_query + "\" has failed.");
        return results;
    } catch (const std::exception &e) {
        logger.logMessage("Error during searchQuery: " + std::string(e.what()));
        logger.logMessage("Search query for Input Query: \"" + filters_query + "\" has failed.");
        return results;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    logger.logUserSearchQuery(filters_query, results.size());
    logger.logSearchPerformanceQuery(filters_query, duration.count());
    return results;


}