#include "widgets/same-year-widget.hpp"
#include <ctime>

bool SameYearWidget::isActive(const std::vector<File>& results, const std::string& query) {
    if (query.find("accesstime") == std::string::npos) return false;
    std::unordered_set<int> years;

    for (const auto& file : results) {
        std::time_t created = file.getCreatedAt();
        std::tm* tm = std::localtime(&created);
        if (tm) {
            years.insert(1900 + tm->tm_year);
        }
    }

    if (years.size() == 1) {
        year = *years.begin();
        active = true;
        return true;
    }

    return false;
}

nlohmann::json SameYearWidget::getData() const {
    return "Year = " + std::to_string(year);
}
