#include <nlohmann/json.hpp>
#include "query-widget-analyzer/file-widget.hpp"

bool FileWidget::isActive(const std::string& query) {
    std::string query_lower = query;
    std::transform(query_lower.begin(), query_lower.end(), query_lower.begin(), ::tolower);
    return query_lower.find("file") != std::string::npos;
}

nlohmann::json FileWidget::getData() const {
    return "File";
}
