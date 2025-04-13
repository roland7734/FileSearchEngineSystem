#include "utils/string-processor.hpp"
#include <stdexcept>
#include <windows.h>

std::string StringProcessor::filterInvalidUtf8Encoding(const std::string& str) {
    std::string result;
    result.reserve(str.size());

    for (size_t idx = 0; idx < str.size();) {
        unsigned char byte = str[idx];

        if ((byte & 0x80) == 0) {
            result.push_back(byte);
            ++idx;
        }
        else if ((byte & 0xE0) == 0xC0 && idx + 1 < str.size() && (str[idx + 1] & 0xC0) == 0x80) {
            result.push_back(byte);
            result.push_back(str[idx + 1]);
            idx += 2;
        }
        else if ((byte & 0xF0) == 0xE0 && idx + 2 < str.size() &&
                 (str[idx + 1] & 0xC0) == 0x80 && (str[idx + 2] & 0xC0) == 0x80) {
            result.push_back(byte);
            result.push_back(str[idx + 1]);
            result.push_back(str[idx + 2]);
            idx += 3;
        }
        else if ((byte & 0xF8) == 0xF0 && idx + 3 < str.size() &&
                 (str[idx + 1] & 0xC0) == 0x80 && (str[idx + 2] & 0xC0) == 0x80 &&
                 (str[idx + 3] & 0xC0) == 0x80) {
            result.push_back(byte);
            result.push_back(str[idx + 1]);
            result.push_back(str[idx + 2]);
            result.push_back(str[idx + 3]);
            idx += 4;
        }
        else {
            result.push_back('?');
            ++idx;
        }
    }

    return result;
}


std::string StringProcessor::convertWideToUtf8(const std::wstring& wstr) {
    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.size()), nullptr, 0, nullptr, nullptr);
    if (bufferSize == 0) {
        throw std::runtime_error("Wide to UTF-8 conversion failed. Error: " + std::to_string(GetLastError()));
    }

    std::string utf8Result(bufferSize, '\0');
    int result = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.size()), &utf8Result[0], bufferSize, nullptr, nullptr);

    if (result == 0) {
        throw std::runtime_error("Wide to UTF-8 conversion failed during the second step. Error: " + std::to_string(GetLastError()));
    }

    return utf8Result;
}


std::string StringProcessor::truncateToMaxSize(const std::string& content, size_t maxSize) {
    if (content.size() > maxSize) {
        return content.substr(0, maxSize);
    }
    return content;
}
std::string StringProcessor::escapeBackslash(const std::string& input) {
    std::string output;
    for (char c : input) {
        if (c == '\\') {
            output += "\\\\";
        } else {
            output += c;
        }
    }
    return output;
}


