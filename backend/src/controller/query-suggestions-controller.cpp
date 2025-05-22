#include "controller/query-suggestions-controller.hpp"
#include <nlohmann/json.hpp>

QuerySuggestionsController::QuerySuggestionsController(SearchHistory* searchHistory)
        : searchHistory(searchHistory) {}

void QuerySuggestionsController::registerRoutes(httplib::Server& server) {
    server.Options("/suggestions", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.status = 204;
    });

    server.Get("/suggestions", [this](const httplib::Request& req, httplib::Response& res) {
        try {
            const auto& suggestionsMap = searchHistory->getSuggestions();

            if (suggestionsMap.empty()) {
                res.set_header("Access-Control-Allow-Origin", "*");
                res.set_content("", "application/json");
                return;
            }

            nlohmann::json json_response;

            for (const auto& [key, list] : suggestionsMap) {
                json_response[key] = nlohmann::json::array();
                for (const auto& item : list) {
                    json_response[key].push_back(item);
                }
            }

            res.set_header("Access-Control-Allow-Origin", "*");
            res.set_content(json_response.dump(), "application/json");

        } catch (const std::exception& e) {
            nlohmann::json error_json;
            error_json["error"] = "Failed to fetch suggestions.";
            error_json["details"] = e.what();
            res.status = 500;
            res.set_header("Access-Control-Allow-Origin", "*");
            res.set_content(error_json.dump(), "application/json");
        }
    });
}
