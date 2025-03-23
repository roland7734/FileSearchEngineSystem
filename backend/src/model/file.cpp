#include <fstream>
#include <sstream>
#include <filesystem>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include "model/file.hpp"

namespace fs = std::filesystem;

File::File(const std::string& path, size_t size, std::time_t createdAt)
        : path(path), size(size), createdAt(createdAt) {
    determineMimeType();
    extractTextContent();
}

File::File(const std::string& path, const std::string& textContent)
        : path(path), textContent(textContent){}

std::string File::getPath() const {
    return path;
}

std::string File::getTextContent() const {
    return textContent;
}

size_t File::getSize() const {
    return size;
}

std::time_t File::getCreatedAt() const {
    return createdAt;
}

std::string File::getMimeType() const {
    return mimeType;
}

std::string File::getName() const {
    return fs::path(path).stem().string();
}

std::string File::getExtension() const {
    return fs::path(path).extension().string();
}


void File::determineMimeType() {

    std::string extension = fs::path(path).extension().string();
    boost::algorithm::to_lower(extension);

    if (extension == ".txt" || extension == ".md" || extension == ".cpp" || extension == ".py") {
        mimeType = "text/plain";
    } else if (extension == ".pdf") {
        mimeType = "application/pdf";
    } else if (extension == ".docx") {
        mimeType = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
    } else {
        mimeType = "application/octet-stream";  // Default for binary files
    }
}

void File::extractTextContent() {
    std::ifstream fileStream(path);
    if (fileStream) {
        std::ostringstream contentStream;
        contentStream << fileStream.rdbuf();
        textContent = contentStream.str();
    }

}
