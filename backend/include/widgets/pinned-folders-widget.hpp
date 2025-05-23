#ifndef PINNED_FOLDERS_WIDGET_HPP
#define PINNED_FOLDERS_WIDGET_HPP

#include "widget.hpp"
#include <vector>
#include <string>

class PinnedFoldersWidget : public Widget {
private:
    std::vector<std::string> topFolders;

public:
    bool isActive(const std::vector<File>& results, const std::string& query) override;
    nlohmann::json getData() const override;
};

#endif
