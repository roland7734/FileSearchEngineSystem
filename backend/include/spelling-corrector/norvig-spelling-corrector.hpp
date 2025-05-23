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
    std::unordered_set<std::string> edits1(const std::string& word);
    std::string mostProbable(const std::unordered_set<std::string>& candidates);
};

#endif
