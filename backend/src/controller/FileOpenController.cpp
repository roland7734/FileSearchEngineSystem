#include <cstdlib>
#include <nlohmann/json.hpp>
#include "controller/FileOpenController.hpp"

FileOpenController::FileOpenController(UsageStatsService* usageStatsService)
        : usageStatsService(usageStatsService) {}

void FileOpenController::registerRoutes(httplib::Server& server) {
    server.Post("/open-file", [this](const httplib::Request& req, httplib::Response& res) {
        auto json = nlohmann::json::parse(req.body, nullptr, false);
        if (json.is_discarded() || !json.contains("path")) {
            res.status = 400;
            res.set_header("Access-Control-Allow-Origin", "*");
            res.set_content("Missing 'path' in request body.", "text/plain");
            return;
        }

        std::string path = json["path"];
        std::string command;

#ifdef _WIN32
        command = "start \"\" \"" + path + "\"";
#elif __APPLE__
        command = "open \"" + path + "\"";
    #else
        command = "xdg-open \"" + path + "\"";
#endif

        int result = std::system(command.c_str());

        res.set_header("Access-Control-Allow-Origin", "*");
        if (result == 0) {
            usageStatsService->incrementAccessCount(path);

            nlohmann::json success = {
                    {"status", "success"},
                    {"message", "File opened successfully."}
            };
            res.set_content(success.dump(), "application/json");
        } else {
            res.status = 500;
            nlohmann::json error = {
                    {"status", "error"},
                    {"message", "Failed to open file."}
            };
            res.set_content(error.dump(), "application/json");
        }
    });

    server.Options("/open-file", [](const httplib::Request&, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.status = 204;
    });
}
