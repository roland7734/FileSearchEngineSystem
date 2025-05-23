#include "widgets/recently-accessed-widget.hpp"

bool RecentlyAccessedWidget::isActive(const std::vector<File>& results, const std::string& query) {
    if (query.find("accesstime") == std::string::npos) return false;
    int count = 0;
    std::time_t now = std::time(nullptr);
    std::time_t limit = now - (30 * 24 * 60 * 60);

    for (const auto& file : results) {
        if (file.getCreatedAt() > limit) {
            if (++count >= 3) {
                active = true;
                return true;
            }
        }
    }
    return false;
}

nlohmann::json RecentlyAccessedWidget::getData() const {
    return "Recently Accessed";
}
