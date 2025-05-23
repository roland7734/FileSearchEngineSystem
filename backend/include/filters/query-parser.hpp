#ifndef QUERY_PARSER_HPP
#define QUERY_PARSER_HPP

#include "ifilter.hpp"
#include <vector>
#include <memory>
#include <string>
#include <unordered_set>
class QueryParser {
public:
    static std::vector <std::unique_ptr<IFilter>> parse(const std::string &query);

private:
    static std::string trim(const std::string &str);
    static std::string extractQuotedValue(const std::string &str);
    static void validateKey(const std::string& key);

    static const std::unordered_set<std::string> validKeys;

    };

#endif // QUERY_PARSER_HPP
