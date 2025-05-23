#ifndef WIDGET_HPP
#define WIDGET_HPP

#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "model/file.hpp"

class Widget {
public:
    virtual ~Widget() = default;
    virtual bool isActive(const std::vector<File>& results, const std::string& query) = 0;
    virtual nlohmann::json getData() const = 0;
};

#endif
