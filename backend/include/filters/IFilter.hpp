#ifndef IFILTER_HPP
#define IFILTER_HPP

#include <string>

class IFilter {
public:
    virtual ~IFilter() = default;
    virtual std::string getKeyword() const = 0;

};

#endif // IFILTER_HPP
