#include <fstream>
#include <sstream>
#include <filesystem>
#include "model/file.hpp"
#include "utils/string-processor.hpp"
#include "config/config.hpp"
#include "utils/magic-wrapper.hpp"

namespace fs = std::filesystem;


File::File(const std::string& path, size_t size, std::time_t createdAt)
        : path(path), size(size), createdAt(createdAt) {
    determineMimeType();
    extractTextContent();
    computeScore();
}

File::File(const std::string& path, const std::string& textContent)
        : path(path), textContent(textContent){}

File::File(const std::string& path, size_t size, std::time_t createdAt, const std::string& mimeType,
          std::string textContent)
: path(path), size(size), createdAt(createdAt), mimeType(mimeType), textContent(textContent){}

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

double File::getScore() const {
    return score;
}

void File::computeScore() {
    score = 0.0;

    int pathDepth = std::count(path.begin(), path.end(), '/');
    score += Config::PATH_DEPTH_WEIGHT / (1 + pathDepth);

    for (const auto& [dir, weight] : Config::DIRECTORY_BONUSES) {
        if (path.find(dir) != std::string::npos) {
            score += weight;
        }
    }

    auto it = std::find(Config::TEXT_EXTENSIONS.begin(), Config::TEXT_EXTENSIONS.end(), getExtension());
    if (it != Config::TEXT_EXTENSIONS.end()) {
        int indexFromEnd = static_cast<int>(Config::TEXT_EXTENSIONS.size() - std::distance(Config::TEXT_EXTENSIONS.begin(), it));
        score += static_cast<double>(indexFromEnd);
    }

    if (size < Config::SMALL_FILE_SIZE) {
        score += Config::SMALL_FILE_BONUS;
    }

    using namespace std::chrono;
    system_clock::time_point createdAtTimePoint = system_clock::from_time_t(createdAt);
    auto now = system_clock::now();
    auto age_in_days = duration_cast<duration<double, std::ratio<86400>>>(now - createdAtTimePoint).count();
    score -= age_in_days * Config::CREATED_TIME_DECAY_PER_DAY;
    if (score < 0.0) score = 0.0;
}




void File::determineMimeType() {

    mimeType = MagicWrapper::instance().detectMimeType(path);
}

void File::extractTextContent() {

    if (mimeType.find("text") != std::string::npos) {
        std::ifstream fileStream(path);
        if (fileStream) {
            std::ostringstream contentStream;
            contentStream << fileStream.rdbuf();
            textContent = StringProcessor::filterInvalidUtf8Encoding(contentStream.str());
        }
    }
}

void File::setScore(double s) { score = s; }
void File::setTextContent(std::string text) { textContent = text; }


