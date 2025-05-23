#include "spelling-corrector/norvig-spelling-corrector.hpp"
#include <regex>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <unordered_set>

NorvigSpellingCorrector::NorvigSpellingCorrector(const std::unordered_map<std::string, int>& freqDict)
        : wordFrequencies(freqDict) {}

std::string NorvigSpellingCorrector::toLower(const std::string& word) {
    std::string res = word;
    std::transform(res.begin(), res.end(), res.begin(), ::tolower);
    return res;
}


std::unordered_set<std::string> NorvigSpellingCorrector::edits1(const std::string& word) {
    static const std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
    std::unordered_set<std::string> edits;
    size_t len = word.length();

    for (size_t i = 0; i <= len; ++i) {
        std::string L = word.substr(0, i);
        std::string R = word.substr(i);
        if (!R.empty()) edits.insert(L + R.substr(1));
        if (R.length() > 1) edits.insert(L + R[1] + R[0] + R.substr(2));
        for (char c : alphabet) {
            if (!R.empty()) edits.insert(L + c + R.substr(1));
            edits.insert(L + c + R);
        }
    }
    return edits;
}

std::string NorvigSpellingCorrector::mostProbable(const std::unordered_set<std::string>& candidates) {
    std::string best = "";
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

    std::unordered_set<std::string> e1 = edits1(lower);
    std::string known1 = mostProbable({e1.begin(), e1.end()});
    if(!known1.empty()) return known1;

    std::unordered_set<std::string> e2;
    for (auto& w : e1) {
        auto e2part = edits1(w);
        std::string known2part = mostProbable(e2part);
        if (!known2part.empty()) e2.insert(known2part);
    }
    auto known2 = mostProbable(e2);
    if (!known2.empty()) return known2;

    return word;
}

std::string NorvigSpellingCorrector::correctQuery(const std::string& query) {
    static const std::unordered_set<std::string> validKeys = {
            "content", "path", "size", "accesstime", "mimetype"
    };
    static const std::unordered_set<std::string> skipValueCorrection = {
            "size", "accesstime", "mimetype", "path"
    };

    std::ostringstream corrected;
    size_t pos = 0;

    while (pos < query.size()) {
        while (pos < query.size() && std::isspace(query[pos])) ++pos;
        if (pos >= query.size()) break;

        size_t keyStart = pos;
        while (pos < query.size() && std::isalnum(query[pos])) ++pos;
        std::string key = query.substr(keyStart, pos - keyStart);

        std::string op;
        if (query.substr(pos, 2) == ">=" || query.substr(pos, 2) == "<=") {
            op = query.substr(pos, 2);
            pos += 2;
        } else if (query[pos] == ':' || query[pos] == '<' || query[pos] == '>' || query[pos] == '=') {
            op = query.substr(pos, 1);
            ++pos;
        } else {
            return query;
        }

        if (validKeys.find(key) == validKeys.end()) return query;

        std::string value;
        bool hadQuotes = false;

        if (pos < query.size() && query[pos] == '"') {
            hadQuotes = true;
            ++pos;
            size_t start = pos;
            while (pos < query.size() && query[pos] != '"') ++pos;
            if (pos >= query.size()) return query;
            value = query.substr(start, pos - start);
            ++pos;
        } else {
            size_t start = pos;
            while (pos < query.size() && !std::isspace(query[pos])) ++pos;
            value = query.substr(start, pos - start);
        }

        if (skipValueCorrection.count(key)) {
            corrected << key << op << (hadQuotes ? "\"" + value + "\"" : value) << " ";
        } else {
            std::istringstream wordStream(value);
            std::ostringstream fixed;
            std::string word;
            bool first = true;

            while (wordStream >> word) {
                std::string corr = correctWord(word);
                if (corr != word) {
                    std::cout << "[Correction] \"" << word << "\" → \"" << corr << "\"\n";
                }
                if (!first) fixed << " ";
                fixed << corr;
                first = false;
            }

            if (hadQuotes)
                corrected << key << op << "\"" << fixed.str() << "\" ";
            else
                corrected << key << op << fixed.str() << " ";
        }
    }

    std::string result = corrected.str();
    if (!result.empty() && result.back() == ' ') result.pop_back();
    return result;
}