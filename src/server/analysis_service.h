#pragma once
#include <nlohmann/json.hpp>
#include "../parser/graph_builder.h"
#include "../detectors/cycle/cycle.h"
#include "../detectors/critical/critical_vertex.h"
#include "../detectors/redundancy/redundancy.h"

namespace DepAnalyser::AnalysisService {

    std::string buildJson(const Graph::Graph& graph, const std::vector<std::unordered_set<const Graph::Vertex *>>& scc,
                          const std::unordered_set<const Graph::Vertex *>& critical,
                          const std::vector<Types::Edge>& redundancy);

    std::string analyse(std::string_view path);

}
