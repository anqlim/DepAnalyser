#include "analysis_service.h"

namespace DepAnalyser::AnalysisService {

    std::string analyse(std::string_view path) {
        Parsing::GraphBuilder builder(path);
        Graph::Graph graph = builder.build();

        Detection::CycleDetector cycles(graph);
        cycles.run();

        Detection::CriticalVertexDetector critical(graph);
        critical.run();

        Detection::RedundancyDetector redundancy(graph, cycles.result());
        redundancy.run();

        return buildJson(graph, cycles.result(), critical.result(), redundancy.result());
    }

    std::string buildJson(const Graph::Graph& graph, const std::vector<std::unordered_set<const Graph::Vertex *>>& scc,
                          const std::unordered_set<const Graph::Vertex *>& critical,
                          const std::vector<Types::Edge>& redundancy) {
        using json = nlohmann::json;
        json result;
        result["edges"] = json::array();
        for (const auto& [name, u] : graph.getVertices()) {
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