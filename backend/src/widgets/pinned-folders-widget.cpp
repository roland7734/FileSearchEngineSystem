#include "widgets/pinned-folders-widget.hpp"
#include <map>
#include <filesystem>
#include <algorithm>
#include <sstream>

bool PinnedFoldersWidget::isActive(const std::vector<File>& results, const std::string& query) {
    std::map<std::string, int> freq;

    for (const auto& file : results) {
        std::filesystem::path path(file.getPath());
        std::filesystem::path fullPath;
        int depth = 0;
        for (const auto& part : path.parent_path()) {
            if (depth++ >= 6) break;
            fullPath /= part;
        }
        freq[fullPath.string()]++;

    }

    std::vector<std::pair<std::string, int>> sorted(freq.begin(), freq.end());
    std::sort(sorted.begin(), sorted.end(), [](auto& a, auto& b) {
        return a.second > b.second;
    });

    for (size_t i = 0; i < std::min<size_t>(3, sorted.size()); ++i) {
        topFolders.push_back(sorted[i].first);
    }

    return !topFolders.empty();
}

nlohmann::json PinnedFoldersWidget::getData() const {
    return topFolders;
}
