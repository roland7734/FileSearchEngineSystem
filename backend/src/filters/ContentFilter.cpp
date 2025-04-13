#include <pqxx/pqxx>
#include "filters/ContentFilter.hpp"

ContentFilter::ContentFilter(const std::string& keyword) : keyword(keyword) {}

std::string ContentFilter::getKeyword() const { return keyword; }

std::string ContentFilter::getWhereClause(pqxx::work& txn) const{
    return "to_tsvector('english', text_content) @@ plainto_tsquery(" + txn.quote(keyword) + ")";
}
std::string ContentFilter::getPrefix() const { return "content"; }