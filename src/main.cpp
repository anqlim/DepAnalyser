#include <crow.h>
#include <nlohmann/json.hpp>
#include "server/analysis_service.h"

int main(int argc, char* argv[]) {
    using namespace DepAnalyser;
    crow::SimpleApp app;

    CROW_ROUTE(app, "/analyse").methods(crow::HTTPMethod::POST)
            ([](const crow::request& req) {
                auto body = nlohmann::json::parse(req.body);
                std::string path = body["path"];

                if (!std::filesystem::exists(path)) {
                    return crow::response(400, "Invalid path");
                }

                return crow::response(200, DepAnalyser::AnalysisService::analyse(path));
            });

    app.port(8080).run();
    return 0;
}
