#ifndef SIZEFILTER_HPP
#define SIZEFILTER_HPP

#include <pqxx/pqxx>
#include "IFilter.hpp"

class SizeFilter : public IFilter {
public:
    explicit SizeFilter(const std::string& input);
    std::string getKeyword() const override;
    std::string getWhereClause(pqxx::work& txn) const override;
    std::string getPrefix() const override;


private:
    std::string rawValue;
    static std::string isNumber(const std::string& value);
};

#endif // SIZEFILTER_HPP
