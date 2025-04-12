#ifndef QUERY_PARSER_HPP
#define QUERY_PARSER_HPP

#include "IFilter.hpp"
#include <vector>
#include <memory>
#include <string>

class QueryParser {
public:
    static std::vector <std::unique_ptr<IFilter>> parse(const std::string &query);

private:
    static std::string trim(const std::string &str);
    static std::string extractQuotedValue(const std::string &str);

};

#endif // QUERY_PARSER_HPP
