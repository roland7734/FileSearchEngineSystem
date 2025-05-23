#include "spelling-corrector/language-model.hpp"
#include <fstream>
#include <regex>
#include <sstream>
#include <algorithm>
#include <nlohmann/json.hpp>
#include <filesystem>

using json = nlohmann::json;

LanguageModel::LanguageModel(const std::string& path) {
    if (std::filesystem::path(path).extension() == ".json") {
        loadFromJson(path);
    } else {
        buildFromTextFile(path);
    }
}

void LanguageModel::loadFromJson(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) {
        throw std::runtime_error("Failed to open cache file: " + filename);
    }

    json j;
    in >> j;
    wordFrequencies = j.get<std::unordered_map<std::string, int>>();
}

void LanguageModel::buildFromTextFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open text file: " + filename);
    }

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

LanguageModel& LanguageModel::getInstance(const std::string& path) {
    static LanguageModel instance(path);
    return instance;
}

const std::unordered_map<std::string, int>& LanguageModel::getWordFrequencies() const {
    return wordFrequencies;
}
