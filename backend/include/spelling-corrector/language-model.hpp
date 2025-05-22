#ifndef LANGUAGEMODEL_HPP
#define LANGUAGEMODEL_HPP

#include <unordered_map>
#include <string>

class LanguageModel {
public:
    static LanguageModel& getInstance(const std::string& corpusPath = "big.txt");

    const std::unordered_map<std::string, int>& getWordFrequencies() const;

private:
    LanguageModel(const std::string& corpusPath);
    std::unordered_map<std::string, int> wordFrequencies;
};

#endif
