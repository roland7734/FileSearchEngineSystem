#include <sstream>
#include <algorithm>
#include <vector>
#include "filters/IFilter.hpp"
#include "filters/QueryParser.hpp"
#include "filters/ContentFilter.hpp"
#include "filters/PathNameFilter.hpp"

std::vector<std::unique_ptr<IFilter>> QueryParser::parse(const std::string& query) {
    std::vector<std::unique_ptr<IFilter>> filters;
    std::string key;
    std::string value;
    size_t pos = 0;

    while (pos < query.length()) {
        while (pos < query.length() && std::isspace(query[pos])) {
            ++pos;
        }

        size_t keyStart = pos;
        size_t keyEnd = query.find(':', keyStart);
        if (keyEnd == std::string::npos)
            throw std::invalid_argument("No ':' character present.");


        key = query.substr(keyStart, keyEnd - keyStart);
        key = trim(key);

        pos = keyEnd + 1;
        if(pos + 2 >= query.length())
            throw std::invalid_argument("No value present for a given key.");


        size_t valueStart = pos;

        if (query[pos] == '"') {
            size_t valueEnd = query.find('"', valueStart + 1);
            if (valueEnd == std::string::npos) {
                throw std::invalid_argument("Unmatched quotes in query.");
            }

            size_t afterQuotePos = valueEnd + 1;
            if (afterQuotePos < query.length() && !std::isspace(query[afterQuotePos]) && query[afterQuotePos] != ':') {
                throw std::invalid_argument("Unexpected characters after quoted value in query.");
            }


            value = query.substr(valueStart + 1, valueEnd - valueStart - 1);
            pos = valueEnd + 1;
        } else {
            size_t valueEnd = query.find_first_of(" \n", valueStart);
            if (valueEnd == std::string::npos) valueEnd = query.length();
            value = query.substr(valueStart, valueEnd - valueStart);
            pos = valueEnd;

            if (value.empty()) {
                if (query[pos] == ':' || query[pos] == '"') {
                    value = "";
                } else if (pos < query.length() && !std::isspace(query[pos]) && query[pos] != ':') {
                    throw std::invalid_argument("Unexpected characters after unquoted value in query.");
                }
            }
        }

        value = trim(value);

        if (key == "content") {
            filters.push_back(std::make_unique<ContentFilter>(value));
        } else if (key == "path") {
            filters.push_back(std::make_unique<PathNameFilter>(value));
        } else {
            throw std::invalid_argument("Unrecognized key in query: " + key);
        }
    }

    return filters;
}

std::string QueryParser::trim(const std::string& str) {
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