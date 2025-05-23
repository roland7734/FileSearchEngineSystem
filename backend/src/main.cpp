#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <sstream>
#include <pqxx/pqxx>
#include "../include/config/config.hpp"
#include "../include/database/database.hpp"
#include "indexer/index-builder.hpp"
#include "model/file.hpp"
#include "service/search-service.hpp"
#include "logger/logger-manager.hpp"
#include "config/config.hpp"
#include "filters/query-parser.hpp"
#include "filters/content-filter.hpp"
#include "filters/path-name-filter.hpp"
#include "controller/search-service-controller.hpp"
#include "controller/crawl-service-controller.hpp"
#include "utils/magic-wrapper.hpp"
#include "controller/file-open-controller.hpp"
#include "observers/results-history.hpp"
#include "observers/search-history.hpp"
#include "controller/query-suggestions-controller.hpp"
#include "controller/logger-controller.hpp"
#include "spelling-corrector/language-model.hpp"
#include <crow/app.h>
#include <crow/middlewares/cors.h>



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
void searchQuery(SearchService& searchService) {
    std::string rawQuery;
    std::cout << "Enter the query: ";
    std::getline(std::cin, rawQuery);

    auto filters = QueryParser::parse(rawQuery);
    std::string filters_query;
    for(const auto& filter : filters)
    if (const auto* contentFilter = dynamic_cast<const ContentFilter*>(filter.get())) {
        filters_query += "content:" + contentFilter->getKeyword() + " ";
    } else if (const auto* nameFilter = dynamic_cast<const PathNameFilter*>(filter.get())) {
        filters_query += "path:" + nameFilter->getKeyword() + " ";
    }
    std::cout<<filters_query;
    auto files = searchService.searchQuery(filters);

    std::cout << "Found " << files.size() << " result(s) for text content search.\n";

    for (auto& file : files) {
        std::cout << file.getPath() << "\n";
        std::cout << "Preview:\n";
        std::cout << file.getTextContent() << "\n\n";
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
        std::cout << "5. Search for Query\n";
        std::cout << "6. Exit\n";
        std::cout << "Enter your choice (1-6): ";
        std::getline(std::cin, input);

        std::stringstream ss(input);
        if (ss >> choice && ss.eof()) {
            return choice;
        } else {
            std::cout << "Invalid command. Please enter a valid number (1-5).\n";
        }
    }
}



int main()
{
    std::filesystem::path current_path = std::filesystem::current_path();
    std::filesystem::path config_path = current_path / "config.json";
    Config::loadConfig(config_path.string());

    std::cout << "Welcome to the File Search Engine\n";

    Database db;
    db.connect();
    std::shared_ptr<ISearchService> baseSearchService = std::make_shared<SearchService>(&db);
    std::shared_ptr<CachedSearchService> cachedSearchService = std::make_shared<CachedSearchService>(baseSearchService);

    InsertService insertService(&db);

    UsageStatsService usageStatsService(&db);
    MonthStatisticsService monthStatisticsService(&db);
    IObserver* searchHistory = new SearchHistory();
    IObserver* resultsHistory = new ResultsHistory(&db);
    std::cout << "Loading big.txt..." << std::endl;
//    auto& freqDict = LanguageModel::getInstance("big.txt").getWordFrequencies();

    cachedSearchService->addObserver(searchHistory);
    cachedSearchService->addObserver(resultsHistory);


    CrawlServiceController crawlServiceController(&insertService);
    SearchServiceController searchServiceController(cachedSearchService.get(), &monthStatisticsService);
    FileOpenController fileOpenController(&usageStatsService);
    QuerySuggestionsController querySuggestionsController(dynamic_cast<SearchHistory*>(searchHistory));
    LoggerController loggerController;

    httplib::Server server;

    fileOpenController.registerRoutes(server);
    crawlServiceController.registerRoutes(server);
    searchServiceController.registerRoutes(server);
    querySuggestionsController.registerRoutes(server);
    loggerController.registerRoutes(server);

    std::cout << "Server is running on http://localhost:18018" << std::endl;
    server.listen("0.0.0.0", 18018);


    return 0;
}
