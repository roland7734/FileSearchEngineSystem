#ifndef CONTENT_FILTER_HPP
#define CONTENT_FILTER_HPP

#include "ifilter.hpp"

class ContentFilter : public IFilter {
private:
    std::string keyword;
public:
    explicit ContentFilter(const std::string& keyword);
    std::string getKeyword() const override;
    std::string getWhereClause(pqxx::work& txn) const override;
    std::string getPrefix() const override;


};

#endif // CONTENT_FILTER_HPP
