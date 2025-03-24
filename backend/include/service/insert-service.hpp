// insert-service.hpp

#ifndef INSERT_SERVICE_HPP
#define INSERT_SERVICE_HPP

#include <vector>
#include "database/database.hpp"
#include "model/file.hpp"

class InsertService {
public:
    InsertService(Database* db);

    bool insertBatchToDatabase(const std::vector<File>& files);
    std::string escapeString(const std::string& str);
    std::string cleanInvalidUtf8Characters(const std::string str);
    std::string WideToUtf8(const std::wstring& wstr);

        private:
    Database* db;
};

#endif  // INSERT_SERVICE_HPP
