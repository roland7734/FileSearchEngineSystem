#ifndef NORVIGSPELLINGCORRECTOR_HPP
#define NORVIGSPELLINGCORRECTOR_HPP

#include "ispelling-correction-strategy.hpp"
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>

class NorvigSpellingCorrector : public ISpellingCorrectionStrategy {
public:
    NorvigSpellingCorrector(const std::unordered_map<std::string, int>& freqDict);

    std::string correctWord(const std::string& word) override;
    std::string correctQuery(const std::string& query) override;

private:
    const std::unordered_map<std::string, int>& wordFrequencies;

    std::string toLower(const std::string& word);
    std::vector<std::string> tokenize(const std::string& text);
    std::unordered_set<std::string> known(const std::vector<std::string>& words);
    std::vector<std::string> edits1(const std::string& word);
    std::string mostProbable(const std::vector<std::string>& candidates);
};

#endif
