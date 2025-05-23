#ifndef QUERY_WIDGET_HPP
#define QUERY_WIDGET_HPP

#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "model/file.hpp"

class QueryWidget {
public:
    virtual ~QueryWidget() = default;
    virtual bool isActive(const std::string& query) = 0;
    virtual nlohmann::json getData() const = 0;
};

#endif
