#ifndef FILE_WIDGET_HPP
#define FILE_WIDGET_HPP

#include <vector>
#include <string>
#include "query-widget.hpp"

class FileWidget : public QueryWidget {
public:
    bool isActive(const std::string& query) override;
    nlohmann::json getData() const override;
};

#endif
