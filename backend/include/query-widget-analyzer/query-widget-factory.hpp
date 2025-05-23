#ifndef QUERY_WIDGET_FACTORY_HPP
#define QUERY_WIDGET_FACTORY_HPP

#include <vector>
#include <string>
#include <nlohmann/json.hpp>

class QueryWidgetFactory {
public:
    static nlohmann::json getWidgets(const std::string& query);
};

#endif
