#ifndef RECENTLY_ACCESSED_WIDGET_HPP
#define RECENTLY_ACCESSED_WIDGET_HPP

#include "widget.hpp"
#include <ctime>

class RecentlyAccessedWidget : public Widget {
private:
    bool active = false;

public:
    bool isActive(const std::vector<File>& results, const std::string& query) override;
    nlohmann::json getData() const override;
};

#endif
