#include <nlohmann/json.hpp>
#include "query-widget-analyzer/query-widget-factory.hpp"
#include "query-widget-analyzer/university-widget.hpp"
#include "query-widget-analyzer/file-widget.hpp"
#include "query-widget-analyzer/query-widget.hpp"

nlohmann::json QueryWidgetFactory::getWidgets(const std::string& query) {
    std::vector<std::unique_ptr<QueryWidget>> allWidgets;
    allWidgets.push_back(std::make_unique<FileWidget>());
    allWidgets.push_back(std::make_unique<UniversityWidget>());

    std::vector<std::string> widgetNames;

    for (const auto& widget : allWidgets) {
        if (widget->isActive(query)) {
            auto data = widget->getData();
            widgetNames.push_back(data.get<std::string>());
        }
    }

    nlohmann::json response;
    response["query_widgets"] = widgetNames;
    return response;
}
