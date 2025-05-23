#ifndef CACHE_GENERATOR_HPP
#define CACHE_GENERATOR_HPP

#include <unordered_map>
#include <string>

class CacheGenerator {
public:
    void generate(const std::string& inputPath = "big.txt", const std::string& outputPath = "big_cache.json");

private:
    std::unordered_map<std::string, int> buildFrequencyMap(const std::string& path);
    void saveToJson(const std::unordered_map<std::string, int>& map, const std::string& filename);
};

#endif
