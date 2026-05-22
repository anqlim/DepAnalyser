#include <crow.h>
#include <nlohmann/json.hpp>
#include "server/analysis_service.h"

int main() {
    using namespace DepAnalyser;
    crow::SimpleApp app;

    const char* redis_host = std::getenv("REDIS_HOST");
    sw::redis::ConnectionOptions conn_opts;
    conn_opts.host = redis_host ? redis_host : "127.0.0.1";
    conn_opts.port = 6379;

    sw::redis::ConnectionPoolOptions pool_opts;
    pool_opts.size = 4;

    auto redis = sw::redis::Redis(conn_opts, pool_opts);

    CROW_ROUTE(app, "/upload").methods(crow::HTTPMethod::POST)
            ([&redis](const crow::request& req) {
                try {
                    crow::multipart::message msg(req);
                    auto& part = msg.parts[0];

                    std::string tmp_zip = "/tmp/upload_" + std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()) + ".zip";
                    std::ofstream out(tmp_zip, std::ios::binary);
                    out.write(part.body.data(), part.body.size());
                    out.close();

                    return crow::response(200, DepAnalyser::AnalysisService::analyseZip(tmp_zip, redis));
                } catch (const std::exception& e) {
                    return crow::response(500, e.what());
                }
            });

    CROW_ROUTE(app, "/analyse-repo").methods(crow::HTTPMethod::POST)
            ([&redis](const crow::request& req) {
                try {
                    auto body = nlohmann::json::parse(req.body);
                    std::string url = body.at("url");
                    return crow::response(200, DepAnalyser::AnalysisService::analyseRepo(url, redis));
                } catch (const nlohmann::json::exception&) {
                    return crow::response(400, "Invalid JSON");
                } catch (const std::exception& e) {
                    return crow::response(500, e.what());
                }
            });

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

    CROW_ROUTE(app, "/")
            ([]() {
                std::ifstream file("frontend/index.html");
                std::string body((std::istreambuf_iterator<char>(file)),
                                 std::istreambuf_iterator<char>());
                crow::response res(200, body);
                res.set_header("Content-Type", "text/html");
                return res;
            });

    app.port(8080).run();
    return 0;
}
