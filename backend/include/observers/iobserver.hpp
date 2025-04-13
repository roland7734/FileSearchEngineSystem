#ifndef IOBSERVER_HPP
#define IOBSERVER_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void update(const std::unordered_map<std::string, std::vector<std::string>>& filters,
                        const std::unordered_set<int>& results) = 0;
};
#endif // IOBSERVER_HPP