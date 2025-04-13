#ifndef FILE_NAME_FILTER_HPP
#define FILE_NAME_FILTER_HPP

#include "ifilter.hpp"

class PathNameFilter : public IFilter {
private:
    std::string keyword;
public:
    explicit PathNameFilter(const std::string& keyword);
    std::string getKeyword() const override;
    std::string getWhereClause(pqxx::work& txn) const override;
    std::string getPrefix() const override;


};

#endif // FILE_NAME_FILTER_HPP
