#include "analysis_service.h"

namespace DepAnalyser::AnalysisService {

    std::string analyse(std::string_view path, sw::redis::Redis& redis) {
        auto key = computeCacheKey(path);

        auto cached = redis.get(key);
        if (cached) return *cached;

        Parsing::GraphBuilder builder(path);
        Graph::Graph graph = builder.build();

        Detection::CycleDetector cycles(graph);
        cycles.run();

        Detection::CriticalVertexDetector critical(graph);
        critical.run();

        Detection::RedundancyDetector redundancy(graph, cycles.result());
        redundancy.run();

        auto result = buildJson(graph, cycles.result(), critical.result(), redundancy.result());
        redis.setex(key, 3600, result);
        return result;
    }

    static std::string computeCacheKey(std::string_view path) {
        std::size_t hash = 0;
        for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
            if (!entry.is_regular_file()) continue;
            auto mtime = std::filesystem::last_write_time(entry).time_since_epoch().count();
            hash ^= std::hash<std::string>{}(entry.path().string()) + mtime;
        }
        return "analysis:" + std::to_string(hash);
    }

    std::string buildJson(const Graph::Graph& graph, const std::vector<std::unordered_set<const Graph::Vertex *>>& scc,
                          const std::unordered_set<const Graph::Vertex *>& critical,
                          const std::vector<Types::Edge>& redundancy) {
        using json = nlohmann::json;
        json result;
        result["edges"] = json::array();
        result["isolated"] = json::array();
        for (const auto& [name, u] : graph.getVertices()) {
            if (u->dependencies.empty() && u->dependents.empty()) {
                result["isolated"].push_back({
                                                     {"id", name},
                                                     {"label", std::filesystem::path(name).filename().string()}
                                             });
            }
            for (const auto& v : u->dependencies) {
                result["edges"].push_back({
                                                  {"from_id", name},
                                                  {"to_id", v->file_name},
                                                  {"from_label", std::filesystem::path(name).filename().string()},
                                                  {"to_label", std::filesystem::path(v->file_name).filename().string()}
                });
            }
        }

        result["cycles"] = json::array();
        for (const auto& component : scc) {
            json cycle = json::array();
            for (const auto& v : component) {
                cycle.push_back({
                                        {"id", v->file_name},
                                        {"label", std::filesystem::path(v->file_name).filename().string()}
                });
            }
            result["cycles"].push_back(cycle);
        }

        result["critical"] = json::array();
        for (const auto& v : critical) {
            result["critical"].push_back({
                                                 {"id", v->file_name},
                                                 {"label", std::filesystem::path(v->file_name).filename().string()}
            });
        }

        result["redundancy"] = json::array();
        for (const auto& e : redundancy) {
            result["redundancy"].push_back({
                                                   {"from_id", e.from->file_name},
                                                   {"to_id", e.to->file_name},
                                                   {"from_label", std::filesystem::path(e.from->file_name).filename().string()},
                                                   {"to_label", std::filesystem::path(e.to->file_name).filename().string()}
            });
        }

        return result.dump();
    }

}