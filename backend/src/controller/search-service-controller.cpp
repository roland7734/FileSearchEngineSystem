#include "controller/search-service-controller.hpp"
#include "service/search-service.hpp"
#include "filters/query-parser.hpp"
#include <httplib.h>
#include <nlohmann/json.hpp>

SearchServiceController::SearchServiceController(SearchService* searchService) : searchService(searchService) {}

void SearchServiceController::registerRoutes(httplib::Server& server) {
    server.Options("/search", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.status = 204;
    });

    server.Get("/search", [this](const httplib::Request& req, httplib::Response& res) {
        try {
            std::string query = req.get_param_value("query");
            if (!query.empty()) {
                auto filters = QueryParser::parse(query);
                std::vector<File> files = searchService->searchQuery(filters);

                nlohmann::json json_response;
                json_response["count"] = files.size();
                json_response["results"] = nlohmann::json::array();

                for (const auto& file : files) {
                    nlohmann::json file_json;
                    file_json["path"] = file.getPath();
                    file_json["preview"] = file.getTextContent();
                    json_response["results"].push_back(file_json);
                }
                res.set_header("Access-Control-Allow-Origin", "*");
                res.set_content(json_response.dump(), "application/json");
            } else {
                nlohmann::json error_json;
                error_json["error"] = "Query parameter missing.";
                res.status = 400;
                res.set_header("Access-Control-Allow-Origin", "*");
                res.set_content(error_json.dump(), "application/json");
            }
        } catch (const std::exception& e) {
            nlohmann::json error_json;
            error_json["error"] = "Error processing the request.";
            error_json["details"] = e.what();
            res.status = 400;
            res.set_header("Access-Control-Allow-Origin", "*");
            res.set_content(error_json.dump(), "application/json");
        }
    });
}
