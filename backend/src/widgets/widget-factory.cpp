#include "widgets/widget-factory.hpp"
#include "widgets/recently-accessed-widget.hpp"
#include "widgets/same-year-widget.hpp"
#include "widgets/pinned-folders-widget.hpp"

nlohmann::json WidgetFactory::getWidgets(const std::vector<File>& results, const std::string& query) {
    std::vector<std::unique_ptr<Widget>> allWidgets;
    allWidgets.push_back(std::make_unique<RecentlyAccessedWidget>());
    allWidgets.push_back(std::make_unique<SameYearWidget>());
    allWidgets.push_back(std::make_unique<PinnedFoldersWidget>());

    std::vector<std::string> widgetNames;
    nlohmann::json folderData = nlohmann::json::array();

    for (const auto& widget : allWidgets) {
        if (widget->isActive(results, query)) {
            auto data = widget->getData();
            if (data.is_string()) {
                widgetNames.push_back(data.get<std::string>());
            } else if (data.is_array()) {
                folderData = data;
            }
        }
    }

    nlohmann::json response;
    response["widgets"] = widgetNames;
    response["folders"] = folderData;
    return response;
}
