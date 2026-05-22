#pragma once
#include <nlohmann/json.hpp>
#include "../parser/graph_builder.h"
#include "../detectors/cycle/cycle.h"
#include "../detectors/critical/critical_vertex.h"
#include "../detectors/redundancy/redundancy.h"
#include <sw/redis++/redis++.h>

namespace DepAnalyser::AnalysisService {

    std::string buildJson(const Graph::Graph& graph, const std::vector<std::unordered_set<const Graph::Vertex *>>& scc,
                          const std::unordered_set<const Graph::Vertex *>& critical,
                          const std::vector<Types::Edge>& redundancy);

    static std::string computeCacheKey(std::string_view path);

    std::string analyse(std::string_view path, sw::redis::Redis& redis);

    std::string analyseRepo(const std::string& github_url, sw::redis::Redis& redis);

    std::string analyseZip(const std::string& zip_path, sw::redis::Redis& redis);

}
