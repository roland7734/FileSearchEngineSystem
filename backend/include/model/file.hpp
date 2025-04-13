#ifndef FILE_HPP
#define FILE_HPP

#include <string>
#include <ctime>

class File {
public:
    File(const std::string& path, size_t size, std::time_t createdAt = std::time(nullptr));
    File(const std::string& path, const std::string& textContent);

    std::string getName() const;
    std::string getExtension() const;
    std::string getPath() const;
    std::string getTextContent() const;
    size_t getSize() const;
    std::time_t getCreatedAt() const;
    std::string getMimeType() const;
    double getScore() const;

private:
    void determineMimeType();
    void extractTextContent();
    void computeScore();

    std::string path;
    size_t size;
    std::time_t createdAt;
    std::string mimeType;
    std::string textContent;
    double score;
};

#endif  // FILE_HPP
