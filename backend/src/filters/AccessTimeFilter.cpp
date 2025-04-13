#include "filters/AccessTimeFilter.hpp"
#include <regex>

AccessTimeFilter::AccessTimeFilter(const std::string& input) : rawValue(input) {}

std::string AccessTimeFilter::getKeyword() const {
    return rawValue;
}

std::string AccessTimeFilter::getWhereClause(pqxx::work& txn) const {
    char op = rawValue[0];
    std::string value = (op == '>' || op == '<' || op == '=') ? rawValue.substr(1) : rawValue;
    std::string operatorStr = (op == '>' || op == '<' || op == '=') ? std::string(1, op) : "=";
    std::string newValue = normalizeDate(value);
    return "created_at " + operatorStr + " " + txn.quote(newValue);
}

std::string AccessTimeFilter::getPrefix() const { return "accesstime"; }

std::string AccessTimeFilter::normalizeDate(const std::string& value) {
    static const std::regex fullDate(R"(^\d{4}-\d{2}-\d{2}$)");
    static const std::regex yearMonth(R"(^\d{4}-\d{2}$)");
    static const std::regex yearOnly(R"(^\d{4}$)");

    if (std::regex_match(value, fullDate)) {
        return value;
    } else if (std::regex_match(value, yearMonth)) {
        return value + "-01";
    } else if (std::regex_match(value, yearOnly)) {
        return value + "-01-01";
    }

    throw std::invalid_argument("Invalid date format: " + value);
}