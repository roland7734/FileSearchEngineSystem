// insert-service.cpp

#include <pqxx/pqxx>
#include "service/insert-service.hpp"
#include "database/database.hpp"
#include "model/file.hpp"
#include <iostream>
#include <sstream>
#include <fstream>

InsertService::InsertService(Database* db) : db(db) {}

void InsertService::insertBatchToDatabase(const std::vector<File>& files) {
    try {
        if (files.empty()) {
            std::cerr << "No files to insert." << std::endl;
            return;
        }

        pqxx::connection* conn = db->getConnection();
        pqxx::work txn(*conn);

        std::string query = "";
        query += "WITH inserted_files AS (";
        query += "INSERT INTO files (path, name, extension, text_content) VALUES ";
        for (size_t i = 0; i < files.size(); ++i) {
            const File& file = files[i];
            if (i > 0) {
                query += ", ";

            }
            query += "('";
            query += file.getPath();
            query += "', '";
            query += file.getName();
            query += "', '";
            query += file.getExtension();
            query += "', '";
            query += escapeString(file.getTextContent());
            query += "')";

        }

        query += " RETURNING id, path ) ";
        query += "INSERT INTO file_metadata (file_id, mime_type, created_at, size) ";

        for (size_t i = 0; i < files.size(); ++i) {
            const File& file = files[i];
            if(i>0) query += "UNION ALL ";

            std::time_t createdAt = file.getCreatedAt();
            std::tm tm = *std::localtime(&createdAt);
            char timeBuffer[20];
            std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", &tm);
            std::string createdAtStr = timeBuffer;

            query += "SELECT id, ' " + file.getMimeType() + "', "
                     + "'"+ createdAtStr + "'::timestamp AS created_at, "
                     + std::to_string(file.getSize()) + " AS size FROM inserted_files WHERE path = '"
                     + escapeString(file.getPath()) + "' ";
        }

        txn.exec(query);
        txn.commit();

        std::cout << "Batch insert of " << files.size() << " files completed successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error during batch insert: " << e.what() << std::endl;
    }
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