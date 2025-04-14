#include "controller/logger-controller.hpp"
#include "logger/html-logger-strategy.hpp"
#include "logger/text-logger-strategy.hpp"
#include "logger/logger-manager.hpp"
#include <nlohmann/json.hpp>

void LoggerController::registerRoutes(httplib::Server& server) {
    server.Options("/logger/strategy", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.status = 204;
    });

    server.Post("/logger/strategy", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");

        try {
            auto json = nlohmann::json::parse(req.body);
            std::string strategy = json.value("type", "");
            std::string path = json.value("path", "");

            if (strategy == "html") {
                LoggerManager::instance().setStrategy(
                        std::make_unique<HtmlLoggerStrategy>(path.empty() ? "logfile.html" : path).release()
                );
            } else if (strategy == "text") {
                LoggerManager::instance().setStrategy(
                        std::make_unique<TextLoggerStrategy>(path.empty() ? "logfile.txt" : path).release()
                );

//                LoggerManager::instance().setStrategy(std::make_unique<TextLoggerStrategy>(path.empty() ? "logfile.txt" : path));
            } else {
                res.status = 400;
                res.set_content(R"({"error":"Invalid logger type. Use 'html' or 'text'."})", "application/json");
                return;
            }

            LoggerManager::instance().logMessage("Logger strategy switched to: " + strategy);

            nlohmann::json response_json = {
                    {"message", "Logger strategy changed successfully"},
                    {"type", strategy}
            };
            res.status = 200;
            res.set_content(response_json.dump(), "application/json");

        } catch (const std::exception& e) {
            res.status = 500;
            nlohmann::json error = {
                    {"error", "Failed to change logger strategy"},
                    {"details", e.what()}
            };
            res.set_content(error.dump(), "application/json");
        }
    });
}
