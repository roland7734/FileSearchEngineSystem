// file-crawler.hpp

#ifndef FILE_CRAWLER_HPP
#define FILE_CRAWLER_HPP

#include "file-ignorer.hpp"
#include "model/file.hpp"
#include <string>
#include <vector>

class FileCrawler {
public:
    FileCrawler(FileIgnorer* ignorer, const std::string& basePath);

    std::vector<File> getFilesRecursively() const;

private:
    FileIgnorer* ignorer;
    const std::string& basePath;
};

#endif  // FILE_CRAWLER_HPP
