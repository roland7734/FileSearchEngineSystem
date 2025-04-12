#include <pqxx/pqxx>
#include "filters/PathNameFilter.hpp"

PathNameFilter::PathNameFilter(const std::string& keyword) : keyword(keyword) {}

std::string PathNameFilter::getKeyword() const { return keyword; }
