#ifndef UNIVERSITY_WIDGET_HPP
#define UNIVERSITY_WIDGET_HPP

#include <vector>
#include <string>
#include "query-widget.hpp"

class UniversityWidget : public QueryWidget {
public:
    bool isActive(const std::string& query) override;
    nlohmann::json getData() const override;
};

#endif
