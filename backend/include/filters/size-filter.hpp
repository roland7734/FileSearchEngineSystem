#ifndef SIZE_FILTER_HPP
#define SIZE_FILTER_HPP

#include <pqxx/pqxx>
#include "ifilter.hpp"

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

#endif // SIZE_FILTER_HPP
