#include "spelling-corrector/language-model.hpp"
#include <fstream>
#include <regex>
#include <sstream>
#include <algorithm>

LanguageModel::LanguageModel(const std::string& corpusPath) {
    std::ifstream file(corpusPath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string text = buffer.str();

    std::regex wordRegex(R"(\w+)");
    auto words_begin = std::sregex_iterator(text.begin(), text.end(), wordRegex);
    auto words_end = std::sregex_iterator();

    for (auto it = words_begin; it != words_end; ++it) {
        std::string word = it->str();
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        wordFrequencies[word]++;
    }
}

LanguageModel& LanguageModel::getInstance(const std::string& corpusPath) {
    static LanguageModel instance(corpusPath);
    return instance;
}

const std::unordered_map<std::string, int>& LanguageModel::getWordFrequencies() const {
    return wordFrequencies;
}
