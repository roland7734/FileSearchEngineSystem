#include "cache/cached-search-service.hpp"
#include <iostream>
#include <unordered_set>
#include <unordered_map>
CachedSearchService::CachedSearchService(std::shared_ptr<ISearchService> service)
        : realService(std::move(service)) {}

std::vector<File> CachedSearchService::searchQuery(const std::vector<std::unique_ptr<IFilter>>& filters) {

    std::string key = generateCacheKey(filters);

    auto it = cache.find(key);
    if (it != cache.end()) {
        std::cout << "[Cache HIT] " << key << "\n";

        usageList.erase(it->second.second);
        usageList.push_front(key);
        it->second.second = usageList.begin();


        std::unordered_map<std::string, std::vector<std::string>> filterMap;
        std::unordered_set<int> fileIds;
        for (const auto& filter : filters) {
            std::string key = filter->getPrefix();
            std::string value = filter->getKeyword();
            filterMap[key].push_back(value);
        }
        for (const auto &row: it->second.first) {
            fileIds.insert(row.getID());
        }
        notifyObservers(filterMap, fileIds);

        return it->second.first;
    }

    std::cout << "[Cache MISS] " << key << "\n";
    std::vector<File> result = realService->searchQuery(filters);

    usageList.push_front(key);
    cache[key] = {result, usageList.begin()};

    if (cache.size() > maxCacheSize) {
        std::string lruKey = usageList.back();
        usageList.pop_back();
        cache.erase(lruKey);
        std::cout << "[LRU EVICT] " << lruKey << "\n";
    }

    return result;
}

std::string CachedSearchService::generateCacheKey(const std::vector<std::unique_ptr<IFilter>>& filters) {
    std::vector<std::string> parts;
    parts.reserve(filters.size());

    for (const auto& filter : filters) {
        parts.emplace_back(filter->getPrefix() + ":" + filter->getKeyword());
    }

    std::sort(parts.begin(), parts.end());

    std::ostringstream oss;
    for (const auto& part : parts) {
        oss << part << '|';
    }

    return oss.str();
}

void CachedSearchService::addObserver(IObserver* observer) {
    observers.push_back(observer);
}

void CachedSearchService::notifyObservers(
        const std::unordered_map<std::string, std::vector<std::string>>& filters,
        const std::unordered_set<int>& fileIds
) {
    for (auto* observer : observers) {
        observer->update(filters, fileIds);
    }
}
