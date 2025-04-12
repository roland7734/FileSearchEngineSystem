#ifndef FILENAMEFILTER_HPP
#define FILENAMEFILTER_HPP

#include "IFilter.hpp"

class PathNameFilter : public IFilter {
private:
    std::string keyword;
public:
    explicit PathNameFilter(const std::string& keyword);
    std::string getKeyword() const override;

};

#endif // FILENAMEFILTER_HPP
