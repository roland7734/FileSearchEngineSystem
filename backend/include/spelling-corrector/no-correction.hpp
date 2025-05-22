#ifndef NOCORRECTION_HPP
#define NOCORRECTION_HPP

#include "ispelling-correction-strategy.hpp"
#include <string>

class NoCorrection : public ISpellingCorrectionStrategy {
public:
    std::string correctWord(const std::string& word) override;
    std::string correctQuery(const std::string& query) override;
};

#endif // NOCORRECTION_HPP
