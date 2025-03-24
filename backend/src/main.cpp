#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <sstream>
#include "../include/config/config.hpp"
#include "../include/database/database.hpp"
#include "indexer/index-builder.hpp"
#include "model/file.hpp"
#include "service/search-service.hpp"
#include "logger/logger.hpp"


void enableVTMode() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    if (GetConsoleMode(hOut, &dwMode)) {
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
    }
}

std::string replaceBoldTagsWithEscapeCodes(const std::string& input) {
    std::string result = input;

    size_t pos = 0;
    while ((pos = result.find("<b>", pos)) != std::string::npos) {
        result.replace(pos, 3, "\033[33m");
        pos += 5;
    }

    pos = 0;
    while ((pos = result.find("</b>", pos)) != std::string::npos) {
        result.replace(pos, 4, "\033[0m");
        pos += 4;
    }

    return result;
}

std::string getBaseDirectoryFromUser() {
    std::string basePath;
    std::cout << "Enter the base directory path (e.g., D:\\Facultate\\3nd Year Sem2\\SD): ";
    std::getline(std::cin, basePath);
    return basePath;
}

std::vector<std::string> getPatternsFromUser() {
    std::vector<std::string> patterns;
    std::string pattern;
    std::cout << "Enter file patterns to ignore (e.g., .git, node_modules directory) separated by spaces (enter 'done' when finished): \n";

    while (true) {
        std::cout << "Pattern (or 'done' to finish): ";
        std::getline(std::cin, pattern);
        if (pattern == "done") {
            break;
        }
        patterns.push_back(pattern);
    }
    return patterns;
}

void crawlOperation(InsertService& insertService) {

    std::string basePath = getBaseDirectoryFromUser();

    std::vector<std::string> patterns = getPatternsFromUser();
    FileIgnorer ignorer(patterns);

    FileCrawler fileCrawler(&ignorer, basePath);
    Indexer indexer(&fileCrawler, &insertService);

    std::cout << "Starting the crawl operation...\n";
    indexer.indexTextFiles();
    std::cout << "Crawl completed.\n";
}

void searchFileNamesOperation(SearchService& searchService) {
    std::string keyword;
    std::cout << "Enter the keyword to search for in file names: ";
    std::getline(std::cin, keyword);


    std::vector<File> files = searchService.searchFileNames(keyword);
    std::cout << "Found " << files.size() << " result(s) for file name search.\n";

    for (auto& file : files) {
        std::cout << replaceBoldTagsWithEscapeCodes(file.getPath()) << "\n";
        std::cout << "Preview:\n";
        std::cout << file.getTextContent() << "\n\n";
    }
}

void searchTextContentOperation(SearchService& searchService) {
    std::string keyword;
    std::cout << "Enter the word or phrase to search for in text content: ";
    std::getline(std::cin, keyword);

    if (keyword.find(' ') != std::string::npos) {
        std::cout << "Invalid input: Please enter a single word (no spaces allowed).\n";
        return;
    }

    std::vector<File> files = searchService.searchTextContentBySingleWord(keyword);
    std::cout << "Found " << files.size() << " result(s) for text content search.\n";

    for (auto& file : files) {
        std::cout << file.getPath() << "\n";
        std::cout << "Preview:\n";
        std::cout << replaceBoldTagsWithEscapeCodes(file.getTextContent()) << "\n\n";
    }
}

void searchMultipleWordsOperation(SearchService& searchService) {
    std::string text;
    std::cout << "Enter the words/phrase to search for in text content (separate by space): ";
    std::getline(std::cin, text);

    std::vector<File> files = searchService.searchTextContentByMultipleWords(text);
    std::cout << "Found " << files.size() << " result(s) for text content search.\n";

    for (auto& file : files) {
        std::cout << file.getPath() << "\n";
        std::cout << "Preview:\n";
        std::cout << replaceBoldTagsWithEscapeCodes(file.getTextContent()) << "\n\n";
    }
}
int getValidChoice() {
    int choice;
    std::string input;

    while (true) {
        std::cout << "\nChoose an operation:\n";
        std::cout << "1. Crawl and Insert Files into the Database\n";
        std::cout << "2. Search for a keyword in file names\n";
        std::cout << "3. Search for a word in text content\n";
        std::cout << "4. Search for multiple words in text content\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter your choice (1-5): ";
        std::getline(std::cin, input);

        std::stringstream ss(input);
        if (ss >> choice && ss.eof()) {
            return choice;
        } else {
            std::cout << "Invalid command. Please enter a valid number (1-5).\n";
        }
    }
}

int main() {
    enableVTMode();

    std::cout << "Welcome to the File Search Engine\n";

    Database db;
    db.connect();
    InsertService insertService(&db);
    SearchService searchService(&db);

    while (true) {

        int choice = getValidChoice();

        switch (choice) {
            case 1:
                crawlOperation(insertService);
                break;
            case 2:
                searchFileNamesOperation(searchService);
                break;
            case 3:
                searchTextContentOperation(searchService);
                break;
            case 4:
                searchMultipleWordsOperation(searchService);
                break;
            case 5:
                std::cout << "Exiting the program.\n";
                db.~Database();
                return 0;
            default:
                std::cout << "Invalid command. Please enter a valid number (1-5).\n";
        }
    }

    return 0;
}
