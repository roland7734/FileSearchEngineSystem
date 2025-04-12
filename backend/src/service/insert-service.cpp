// insert-service.cpp

#include <pqxx/pqxx>
#include "service/insert-service.hpp"
#include "database/database.hpp"
#include "model/file.hpp"
#include "logger/logger.hpp"
#include "utils/string-processor.hpp"
#include "config/config.hpp"
#include <iostream>
#include <sstream>
#include <fstream>

InsertService::InsertService(Database* db) : db(db) {}

bool InsertService::insertBatchToDatabase(const std::vector<File>& files) {
    try {
        if (files.empty()) {
            logger.logMessage("No files to insert.");
            return true;
        }

        pqxx::connection* conn = db->getConnection();
        pqxx::work txn(*conn);

        std::string query = "";
        query += "WITH inserted_files AS (";
        query += "INSERT INTO files (path, name, extension, text_content, score) VALUES ";

        for (size_t i = 0; i < files.size(); ++i) {
            const File& file = files[i];
            if (i > 0) {
                query += ", ";
            }

            std::string truncateTextContent = StringProcessor::truncateToMaxSize(file.getTextContent(), Config::MAXIMUM_CONTENT_SIZE);

            query += "(" + txn.quote(escapeString(file.getPath())) + ", " + txn.quote(escapeString(file.getName())) +
                    ", " + txn.quote(escapeString(file.getExtension())) + ", " + txn.quote(truncateTextContent) +
                    ", " + std::to_string(file.getScore()) + ")";

        }

        query += " ON CONFLICT (path) DO UPDATE SET ";
        query += "name = EXCLUDED.name, extension = EXCLUDED.extension, text_content = EXCLUDED.text_content, score=EXCLUDED.score";
        query += " RETURNING id, path) ";

        query += "INSERT INTO file_metadata (file_id, mime_type, created_at, size) ";

        for (size_t i = 0; i < files.size(); ++i) {
            const File& file = files[i];
            if (i > 0) query += " UNION ALL ";

            std::time_t createdAt = file.getCreatedAt();
            std::tm tm = *std::localtime(&createdAt);
            char timeBuffer[20];
            std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", &tm);
            std::string createdAtStr = timeBuffer;

            query += "SELECT id, '" + escapeString(file.getMimeType()) + "', '" + createdAtStr + "'::timestamp, " + std::to_string(file.getSize());
            query += " FROM inserted_files WHERE path = '" + escapeString(file.getPath()) + "'";

        }

        query += " ON CONFLICT (file_id) DO UPDATE SET ";
        query += "mime_type = EXCLUDED.mime_type, created_at = EXCLUDED.created_at, size = EXCLUDED.size";

        txn.exec(query);
        txn.commit();

        logger.logMessage("Batch insert of " + std::to_string(files.size()) + " files completed successfully.");
        return true;
    } catch (const pqxx::sql_error& e) {
        logger.logMessage("SQL error during batch insert: " + std::string(e.what()));
    } catch (const pqxx::broken_connection& e) {
        logger.logMessage("Database connection error: " + std::string(e.what()));
    } catch (const std::exception& e) {
        logger.logMessage("Error during batch insert: " + std::string(e.what()));
    }
    return false;
}

std::string InsertService::escapeString(const std::string& str) {
    std::string escapedStr = str;
    size_t pos = 0;
    while ((pos = escapedStr.find("'", pos)) != std::string::npos) {
        escapedStr.insert(pos, "'");
        pos += 2;
    }
    return escapedStr;
}

