#ifndef FILE_IGNORER_HPP
#define FILE_IGNORER_HPP

#include <vector>
#include <string>

class FileIgnorer {
public:
    FileIgnorer(const std::vector<std::string>& patterns);
    bool shouldIgnore(const std::string& filename);

private:
    std::vector<std::string> ignore_patterns;
};

#endif  // FILE_IGNORER_HPP
