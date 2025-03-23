// insert-service.hpp

#ifndef INSERT_SERVICE_HPP
#define INSERT_SERVICE_HPP

#include <vector>
#include "database/database.hpp"
#include "model/file.hpp"

class InsertService {
public:
    InsertService(Database* db);

    void insertBatchToDatabase(const std::vector<File>& files);
    std::string escapeString(const std::string& str);

private:
    Database* db;
};

#endif  // INSERT_SERVICE_HPP
