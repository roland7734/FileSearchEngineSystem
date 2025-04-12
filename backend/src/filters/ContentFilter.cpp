#include <pqxx/pqxx>
#include "filters/ContentFilter.hpp"

ContentFilter::ContentFilter(const std::string& keyword) : keyword(keyword) {}

std::string ContentFilter::getKeyword() const { return keyword; }

