#ifndef IFILTER_HPP
#define IFILTER_HPP
#include <pqxx/pqxx>
#include <string>

class IFilter {
public:
    virtual ~IFilter() = default;
    virtual std::string getKeyword() const = 0;
    virtual std::string getWhereClause(pqxx::work& txn) const = 0;
    virtual std::string getPrefix() const = 0;


};

#endif // IFILTER_HPP
