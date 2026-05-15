#include <iostream>
#include "parser/graph_builder.h"
#include "detectors/cycle/cycle.h"
#include "detectors/critical/critical_vertex.h"
#include "detectors/redundancy/redundancy.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "[Input format]: DepAnalyser <path>";
        return -1;
    }

    using namespace DepAnalyser;

    std::string path = argv[1];
    if (!std::filesystem::exists(path)) {
        std::cout << "Incorrect path";
        return -1;
    }

    Parsing::GraphBuilder builder(path);
    Graph::Graph graph = std::move(builder.build());
    if (graph.getVertices().empty()) {
        std::cout << "Empty project";
        return 0;
    }

    Detection::CycleDetector cycleDetector(graph);
    cycleDetector.run();
    auto scc = cycleDetector.result();
    std::cout << "Cycles:" << std::endl;
    for (auto component : scc) {
        for (auto v : component) {
            std::cout << std::filesystem::path(v->file_name).filename().string() << std::endl;
        }
        std::cout << std::endl;
    }
    Detection::CriticalVertexDetector criticalDetector(graph);
    criticalDetector.run();
    auto critical_vertices = criticalDetector.result();
    std::cout << "Critical vertices:" << std::endl;
    for (auto v : critical_vertices) {
        std::cout << std::filesystem::path(v->file_name).filename().string() << std::endl;
    }
    std::cout << std::endl;
    Detection::RedundancyDetector redundancyDetector(graph, scc);
    redundancyDetector.run();
    auto redundancy_edges = redundancyDetector.result();
    std::cout << "Redundancy edges:" << std::endl;
    for (auto e : redundancy_edges) {
        std::cout << std::filesystem::path(e.from->file_name).filename().string()
        << "->" << std::filesystem::path(e.to->file_name).filename().string() << std::endl;
    }

    return 0;
}
