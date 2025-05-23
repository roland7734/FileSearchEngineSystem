#ifndef WIDGET_FACTORY_HPP
#define WIDGET_FACTORY_HPP

#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include "model/file.hpp"

class WidgetFactory {
public:
    static nlohmann::json getWidgets(const std::vector<File>& results, const std::string& query);
};

#endif
