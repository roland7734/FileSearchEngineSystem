#include <sstream>
#include <algorithm>
#include <vector>
#include "filters/IFilter.hpp"
#include "filters/QueryParser.hpp"
#include "filters/ContentFilter.hpp"
#include "filters/PathNameFilter.hpp"
#include "filters/SizeFilter.hpp"
#include "filters/MimeTypeFilter.hpp"
#include "filters/FilterField.hpp"
#include "filters/AccessTimeFilter.hpp"
#include <regex>



std::vector<std::unique_ptr<IFilter>> QueryParser::parse(const std::string& query) {
    std::vector<std::unique_ptr<IFilter>> filters;
    size_t pos = 0;

    while (pos < query.length()) {
        while (pos < query.length() && std::isspace(query[pos])) ++pos;
        if (pos >= query.length()) break;

        size_t keyStart = pos;
        size_t sep = query.find_first_of(":<=>", keyStart);
        if (sep == std::string::npos)
            throw std::invalid_argument("Missing separator (:, <, >, =) in query.");

        std::string key = query.substr(keyStart, sep - keyStart);
        key = trim(key);
        if (key.empty()) throw std::invalid_argument("Empty key in query.");

        char op = query[sep];
        ++sep;

        if((key == "size" || key == "accesstime") && op == ':' && sep < query.size())
            if(query[sep] == '=' || query[sep] == '<' || query[sep] == '>') {
                    op = query[sep];
                    ++sep;
                } else op = '=';

        if (sep < query.size() && (query[sep] == '=' || query[sep] == '<' || query[sep] == '>')) {
            throw std::invalid_argument("Double comparison operators like >=, <= are not supported.");
        }

        if (sep >= query.length()) throw std::invalid_argument("Missing value after key.");

        std::string value;
        if (query[sep] == '"') {
            size_t quoteEnd = query.find('"', sep + 1);
            if (quoteEnd == std::string::npos)
                throw std::invalid_argument("Unmatched quote in query.");
            value = query.substr(sep + 1, quoteEnd - sep - 1);
            pos = quoteEnd + 1;
        } else {
            size_t valueEnd = query.find_first_of(" \n", sep);
            if (valueEnd == std::string::npos) valueEnd = query.length();
            value = query.substr(sep, valueEnd - sep);
            pos = valueEnd;
        }

        value = trim(value);
        if (value.empty())
            throw std::invalid_argument("Empty value for key: " + key);

        FilterField fieldType = parseFilterField(key);

        if (fieldType == FilterField::Content) {
            filters.push_back(std::make_unique<ContentFilter>(value));
        } else if (fieldType == FilterField::Path) {
            filters.push_back(std::make_unique<PathNameFilter>(value));
        } else if (fieldType == FilterField::Size) {
            filters.push_back(std::make_unique<SizeFilter>(op + value));
        } else if (fieldType == FilterField::AccessTime) {
            filters.push_back(std::make_unique<AccessTimeFilter>(op + value));
        } else if (fieldType == FilterField::MimeType) {
            filters.push_back(std::make_unique<MimeTypeFilter>(value));
        } else {
            throw std::invalid_argument("Unknown filter field in query: " + key);
        }
    }

    return filters;
}


std::string QueryParser::trim(const std::string& str) {

    if(str.empty()) return "";
    size_t start = 0;
    size_t end = str.size() - 1;

    while (start < str.size() && std::isspace(str[start])) {
        ++start;
    }

    while (end > start && std::isspace(str[end])) {
        --end;
    }

    return str.substr(start, end - start + 1);
}

std::string QueryParser::extractQuotedValue(const std::string& str) {
    if (str.front() == '"' && str.back() == '"') {
        return str.substr(1, str.length() - 2);
    }
    return str;
}