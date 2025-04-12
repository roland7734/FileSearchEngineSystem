#include "controller/CrawlServiceController.hpp"
#include "service/insert-service.hpp"
#include <httplib.h>
#include <nlohmann/json.hpp>

CrawlServiceController::CrawlServiceController(InsertService* insertService)
        : insertService(insertService) {}

void CrawlServiceController::registerRoutes(httplib::Server& server) {
    server.Post("/crawl", [this](const httplib::Request& req, httplib::Response& res) {
        try {
            // Parse the JSON body
            auto body = nlohmann::json::parse(req.body, nullptr, false);
            if (body.is_discarded() || !body.contains("basePath") || !body.contains("patterns")) {
                res.status = 400;
                res.set_content("Missing required fields: basePath or patterns.", "text/plain");
                return;
            }

            std::string basePath = body["basePath"];
            std::vector<std::string> patterns;
            for (const auto& item : body["patterns"]) {
                patterns.emplace_back(item.get<std::string>());
            }

            FileIgnorer ignorer(patterns);
            FileCrawler fileCrawler(&ignorer, basePath);
            Indexer indexer(&fileCrawler, insertService);

            indexer.indexTextFiles();

            nlohmann::json result;
            result["status"] = "success";
            result["message"] = "Crawl completed.";
            res.set_content(result.dump(), "application/json");
        } catch (const std::exception& e) {
            nlohmann::json error;
            error["status"] = "error";
            error["message"] = e.what();
            res.status = 500;
            res.set_content(error.dump(), "application/json");
        }
    });
}
