#include "spelling-corrector/no-correction.hpp"

std::string NoCorrection::correctWord(const std::string& word) {
    return word;
}

std::string NoCorrection::correctQuery(const std::string& query) {
    return query;
}
