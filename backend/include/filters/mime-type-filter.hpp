#ifndef MIME_TYPE_FILTER_HPP
#define MIME_TYPE_FILTER_HPP

#include <pqxx/pqxx>
#include "ifilter.hpp"

class MimeTypeFilter : public IFilter {
public:
    explicit MimeTypeFilter(const std::string& input);
    std::string getKeyword() const override;
    std::string getWhereClause(pqxx::work& txn) const override;
    std::string getPrefix() const override;

private:
    std::string keyword;
};

#endif // MIME_TYPE_FILTER_HPP
