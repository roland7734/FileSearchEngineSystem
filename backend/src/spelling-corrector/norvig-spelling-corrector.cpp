#include "spelling-corrector/norvig-spelling-corrector.hpp"
#include <regex>
#include <algorithm>
#include <sstream>

NorvigSpellingCorrector::NorvigSpellingCorrector(const std::unordered_map<std::string, int>& freqDict)
        : wordFrequencies(freqDict) {}

std::string NorvigSpellingCorrector::toLower(const std::string& word) {
    std::string res = word;
    std::transform(res.begin(), res.end(), res.begin(), ::tolower);
    return res;
}

std::vector<std::string> NorvigSpellingCorrector::tokenize(const std::string& text) {
    std::vector<std::string> tokens;
    std::regex wordRegex(R"(\w+)");
    auto begin = std::sregex_iterator(text.begin(), text.end(), wordRegex);
    auto end = std::sregex_iterator();

    for (auto it = begin; it != end; ++it)
        tokens.push_back(it->str());
    return tokens;
}

std::unordered_set<std::string> NorvigSpellingCorrector::known(const std::vector<std::string>& words) {
    std::unordered_set<std::string> result;
    for (const auto& w : words)
        if (wordFrequencies.find(w) != wordFrequencies.end())
            result.insert(w);
    return result;
}

std::vector<std::string> NorvigSpellingCorrector::edits1(const std::string& word) {
    static const std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
    std::vector<std::string> edits;
    size_t len = word.length();

    for (size_t i = 0; i <= len; ++i) {
        std::string L = word.substr(0, i);
        std::string R = word.substr(i);
        if (!R.empty()) edits.push_back(L + R.substr(1));
        if (R.length() > 1) edits.push_back(L + R[1] + R[0] + R.substr(2));
        for (char c : alphabet) {
            if (!R.empty()) edits.push_back(L + c + R.substr(1));
            edits.push_back(L + c + R);
        }
    }
    return edits;
}

std::string NorvigSpellingCorrector::mostProbable(const std::vector<std::string>& candidates) {
    std::string best = candidates[0];
    int maxFreq = 0;

    for (const auto& w : candidates) {
        int freq = wordFrequencies.count(w) ? wordFrequencies.at(w) : 0;
        if (freq > maxFreq) {
            maxFreq = freq;
            best = w;
        }
    }
    return best;
}

std::string NorvigSpellingCorrector::correctWord(const std::string& word) {
    std::string lower = toLower(word);
    if (wordFrequencies.count(lower)) return lower;

    std::vector<std::string> e1 = edits1(lower);
    auto known1 = known(e1);
    if (!known1.empty()) return mostProbable({known1.begin(), known1.end()});

    std::vector<std::string> e2;
    for (auto& w : e1) {
        auto e2part = edits1(w);
        e2.insert(e2.end(), e2part.begin(), e2part.end());
    }
    auto known2 = known(e2);
    if (!known2.empty()) return mostProbable({known2.begin(), known2.end()});

    return word;
}

std::string NorvigSpellingCorrector::correctQuery(const std::string& query) {
    std::stringstream ss(query);
    std::string token;
    std::string correctedQuery;

    while (ss >> token) {
        size_t colonPos = token.find(':');

        if (colonPos == std::string::npos) {
            correctedQuery += correctWord(token) + " ";
            continue;
        }

        std::string key = token.substr(0, colonPos);
        std::string value = token.substr(colonPos + 1);

        std::string op, valuePart;
        std::smatch match;
        std::regex opRegex(R"((>=|<=|>|<)?(.*))");

        if (std::regex_match(value, match, opRegex)) {
            op = match[1];
            valuePart = match[2];
        } else {
            valuePart = value;
        }

        bool isQuoted = !valuePart.empty() && valuePart.front() == '"';
        bool isNumeric = std::all_of(valuePart.begin(), valuePart.end(), ::isdigit);

        std::string correctedValue = (isQuoted || isNumeric || valuePart.empty())
                                     ? valuePart
                                     : correctWord(valuePart);

        correctedQuery += key + ":" + op + correctedValue + " ";
    }

    if (!correctedQuery.empty()) {
        correctedQuery.pop_back();
    }

    return correctedQuery;
}
