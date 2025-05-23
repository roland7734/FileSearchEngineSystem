#include <fstream>
#include <iostream>
#include <regex>
#include <algorithm>
#include <sstream>
#include <nlohmann/json.hpp>
#include "cache/cache_generator.hpp"

using json = nlohmann::json;

std::unordered_map<std::string, int> CacheGenerator::buildFrequencyMap(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + path);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string text = buffer.str();

    std::unordered_map<std::string, int> frequencies;
    std::regex wordRegex(R"(\w+)");
    auto words_begin = std::sregex_iterator(text.begin(), text.end(), wordRegex);
    auto words_end = std::sregex_iterator();

    for (auto it = words_begin; it != words_end; ++it) {
        std::string word = it->str();
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        frequencies[word]++;
    }

    return frequencies;
}

void CacheGenerator::saveToJson(const std::unordered_map<std::string, int>& map, const std::string& filename) {
    json j(map);
    std::ofstream out(filename);
    if (!out) throw std::runtime_error("Cannot write to file: " + filename);
    out << j.dump(2);
}

void CacheGenerator::generate(const std::string& inputPath, const std::string& outputPath) {
    std::cout << "Processing " << inputPath << "...\n";
    auto freqMap = buildFrequencyMap(inputPath);

    std::cout << "Saving to " << outputPath << "...\n";
    saveToJson(freqMap, outputPath);

    std::cout << "Done. Word count: " << freqMap.size() << "\n";
}
