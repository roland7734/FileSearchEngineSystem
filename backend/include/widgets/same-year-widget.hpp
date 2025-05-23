#ifndef SAME_YEAR_WIDGET_HPP
#define SAME_YEAR_WIDGET_HPP

#include "widget.hpp"
#include <unordered_set>

class SameYearWidget : public Widget {
private:
    int year = -1;
    bool active = false;

public:
    bool isActive(const std::vector<File>& results, const std::string& query) override;
    nlohmann::json getData() const override;
};

#endif
