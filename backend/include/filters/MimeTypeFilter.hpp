#ifndef MIMETYPEFILTER_HPP
#define MIMETYPEFILTER_HPP

#include <pqxx/pqxx>
#include "IFilter.hpp"

class MimeTypeFilter : public IFilter {
public:
    explicit MimeTypeFilter(const std::string& input);
    std::string getKeyword() const override;
    std::string getWhereClause(pqxx::work& txn) const override;
    std::string getPrefix() const override;

private:
    std::string keyword;
};

#endif // MIMETYPEFILTER_HPP
