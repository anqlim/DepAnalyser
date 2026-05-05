#pragma once
#include "../src/graph/graph.h"
using namespace DepAnalyser;

// вспомогательная функция — строит граф быстро
inline Graph::Graph buildGraph(std::vector<std::pair<std::string,std::string>> edges) {
    Graph::Graph g;
    for (auto& [from, to] : edges) {
        if (!g.findVertex(from)) g.addVertex(from);
        if (!g.findVertex(to)) g.addVertex(to);
        g.addEdge(g.findVertex(from), g.findVertex(to));
    }
    return g;
}
