#ifndef LANGUAGEMODEL_HPP
#define LANGUAGEMODEL_HPP

#include <unordered_map>
#include <string>

class LanguageModel {
public:
    static LanguageModel& getInstance(const std::string& path = "big_cache.json");

    const std::unordered_map<std::string, int>& getWordFrequencies() const;

private:
    LanguageModel(const std::string& path);
    void loadFromJson(const std::string& filename);
    void buildFromTextFile(const std::string& filename);

    std::unordered_map<std::string, int> wordFrequencies;
};

#endif
