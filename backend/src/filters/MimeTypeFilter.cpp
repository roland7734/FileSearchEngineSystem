#include "filters/MimeTypeFilter.hpp"

MimeTypeFilter::MimeTypeFilter(const std::string& input) : keyword(input) {}

std::string MimeTypeFilter::getKeyword() const {
    return keyword;
}

std::string MimeTypeFilter::getWhereClause(pqxx::work& txn) const {
    return "mime_type = " + txn.quote(keyword);
}
std::string MimeTypeFilter::getPrefix() const { return "mimetype"; }