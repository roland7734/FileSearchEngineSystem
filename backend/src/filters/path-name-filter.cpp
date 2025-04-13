#include <pqxx/pqxx>
#include "filters/path-name-filter.hpp"
#include "utils/string-processor.hpp"

PathNameFilter::PathNameFilter(const std::string& keyword) : keyword(keyword) {}

std::string PathNameFilter::getKeyword() const { return keyword; }
std::string PathNameFilter::getWhereClause(pqxx::work& txn) const{
    return "path ILIKE " + txn.quote(StringProcessor::escapeBackslash(keyword)+ "%");
}
std::string PathNameFilter::getPrefix() const { return "path"; }