#ifndef CONTENTFILTER_HPP
#define CONTENTFILTER_HPP

#include "IFilter.hpp"

class ContentFilter : public IFilter {
private:
    std::string keyword;
public:
    explicit ContentFilter(const std::string& keyword);
    std::string getKeyword() const override;
    std::string getWhereClause(pqxx::work& txn) const override;
    std::string getPrefix() const override;


};

#endif // CONTENTFILTER_HPP
