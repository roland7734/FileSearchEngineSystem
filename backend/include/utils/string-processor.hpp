// StringProcessor.hpp

#ifndef STRINGPROCESSOR_HPP
#define STRINGPROCESSOR_HPP

#include <string>

class StringProcessor {
public:
    static std::string filterInvalidUtf8Encoding(const std::string& str);
    static std::string convertWideToUtf8(const std::wstring& wstr);
    static std::string truncateToMaxSize(const std::string& content, size_t maxSize);


};

#endif // STRINGPROCESSOR_HPP