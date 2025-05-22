#ifndef ISEARCH_SERVICE_HPP
#define ISEARCH_SERVICE_HPP

#include <vector>
#include <memory>
#include "model/file.hpp"
#include "filters/ifilter.hpp"
#include "observers/iobserver.hpp"

class ISearchService {
public:
    virtual std::vector<File> searchQuery(const std::vector<std::unique_ptr<IFilter>>& filters) = 0;
    virtual void addObserver(IObserver* observer) = 0;
    virtual ~ISearchService() = default;
};

#endif
