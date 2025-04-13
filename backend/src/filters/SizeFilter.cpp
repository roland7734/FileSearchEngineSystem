#include "filters/SizeFilter.hpp"
#include <regex>

SizeFilter::SizeFilter(const std::string& input) : rawValue(input) {}

std::string SizeFilter::getKeyword() const {
    return rawValue;
}

std::string SizeFilter::getWhereClause(pqxx::work& txn) const {
    char op = rawValue[0];
    std::string value = (op == '>' || op == '<' || op == '=') ? rawValue.substr(1) : rawValue;
    std::string operatorStr = (op == '>' || op == '<' || op == '=') ? std::string(1, op) : "=";
    std::string newValue = isNumber(value);
    return "size " + operatorStr + " " + txn.quote(newValue);
}
std::string SizeFilter::getPrefix() const { return "size"; }

std::string SizeFilter::isNumber(const std::string& value)
{
    std::regex numberPattern("^\\d+$");
    if (!std::regex_match(value, numberPattern)) {
        throw std::invalid_argument("Invalid number format: " + value);
    }
    return value;
}
