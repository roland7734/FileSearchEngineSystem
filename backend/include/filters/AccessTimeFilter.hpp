#ifndef ACCESSTIMEFILTER_HPP
#define ACCESSTIMEFILTER_HPP

#include <pqxx/pqxx>
#include "IFilter.hpp"

class AccessTimeFilter : public IFilter {
public:
    explicit AccessTimeFilter(const std::string& input);
    std::string getKeyword() const override;
    std::string getWhereClause(pqxx::work& txn) const override;
    std::string getPrefix() const override;


private:
    std::string rawValue;
    static std::string normalizeDate(const std::string& value);

};

#endif // ACCESSTIMEFILTER_HPP
