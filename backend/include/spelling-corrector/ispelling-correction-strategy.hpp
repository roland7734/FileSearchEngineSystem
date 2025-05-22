#ifndef ISPELLINGCORRECTIONSTRATEGY_HPP
#define ISPELLINGCORRECTIONSTRATEGY_HPP

#include <string>

class ISpellingCorrectionStrategy {
public:
    virtual ~ISpellingCorrectionStrategy() = default;

    virtual std::string correctWord(const std::string& word) = 0;
    virtual std::string correctQuery(const std::string& query) = 0;
};

#endif
