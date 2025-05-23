#include <nlohmann/json.hpp>
#include "query-widget-analyzer/university-widget.hpp"

bool UniversityWidget::isActive(const std::string& query) {
    std::string query_lower = query;
    std::transform(query_lower.begin(), query_lower.end(), query_lower.begin(), ::tolower);
    return query_lower.find("facultate") != std::string::npos;}

nlohmann::json UniversityWidget::getData() const {
    return "University";
}
