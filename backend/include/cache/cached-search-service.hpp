#ifndef CACHED_SEARCH_SERVICE_HPP
#define CACHED_SEARCH_SERVICE_HPP

#include <unordered_map>
#include <vector>
#include <memory>
#include <string>
#include "service/isearch-service.hpp"
#include "model/file.hpp"
#include "filters/ifilter.hpp"
#include <list>

class CachedSearchService : public ISearchService {
public:
    explicit CachedSearchService(std::shared_ptr<ISearchService> service);
    void addObserver(IObserver* observer) override;
    std::vector<File> searchQuery(const std::vector<std::unique_ptr<IFilter>>& filters) override;


private:
    std::shared_ptr<ISearchService> realService;
    size_t maxCacheSize = 100;
    std::list<std::string> usageList;
    std::unordered_map<std::string, std::pair<std::vector<File>, std::list<std::string>::iterator>> cache;
    std::vector<IObserver*> observers;
    void notifyObservers(const std::unordered_map<std::string, std::vector<std::string>>& filters, const std::unordered_set<int>& fileIds);
    std::string generateCacheKey(const std::vector<std::unique_ptr<IFilter>>& filters);
};

#endif // CACHED_SEARCH_SERVICE_HPP
