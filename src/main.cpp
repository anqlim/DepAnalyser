#include <crow.h>
#include <nlohmann/json.hpp>
#include "server/analysis_service.h"

int main(int argc, char* argv[]) {
    using namespace DepAnalyser;
    crow::SimpleApp app;

    sw::redis::ConnectionOptions conn_opts;
    conn_opts.host = "127.0.0.1";
    conn_opts.port = 6379;
    sw::redis::ConnectionPoolOptions pool_opts;
    pool_opts.size = 4;
    auto redis = sw::redis::Redis(conn_opts, pool_opts);

    CROW_ROUTE(app, "/analyse").methods(crow::HTTPMethod::POST)
            ([&redis](const crow::request& req) {
                try {
                    auto body = nlohmann::json::parse(req.body);
                    std::string path = body.at("path");

                    if (!std::filesystem::exists(path)) {
                        return crow::response(400, "Invalid path");
                    }

                    return crow::response(200, DepAnalyser::AnalysisService::analyse(path, redis));
                }
                catch (const nlohmann::json::exception&) {
                    return crow::response(400, "Invalid JSON");
                }
                catch (const std::exception& e) {
                    return crow::response(500, e.what());
                }
            });

    app.port(8080).run();
    return 0;
}
