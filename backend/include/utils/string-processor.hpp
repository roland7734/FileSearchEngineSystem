#ifndef STRING_PROCESSOR_HPP
#define STRING_PROCESSOR_HPP

#include <string>

class StringProcessor {
public:
    static std::string filterInvalidUtf8Encoding(const std::string& str);
    static std::string convertWideToUtf8(const std::wstring& wstr);
    static std::string truncateToMaxSize(const std::string& content, size_t maxSize);
    static std::string escapeBackslash(const std::string& input);

    };

#endif // STRING_PROCESSOR_HPP