#pragma once
#include "detector.h"
#include <stack>
#include <queue>

namespace DepAnalyser::Detection {

    class CriticalVertexDetector : public Detector {
    public:
        explicit CriticalVertexDetector(const Graph::Graph& graph) : Detector(graph) {}
        void run() override;
        [[nodiscard]] const std::unordered_set<const Graph::Vertex*>& result() const;

    private:
        struct BFSData {
            std::stack<const Graph::Vertex*> stack;
            std::unordered_map<const Graph::Vertex*, int> sigma; // число кратчайших расстояний
            std::unordered_map<const Graph::Vertex*, int> dist; // кратчайшее расстояние
            std::unordered_map<const Graph::Vertex*, std::vector<const Graph::Vertex*>> predecessors; // предшественники
        };
        std::unordered_map<const Graph::Vertex*, double> centrality_;
        std::vector<std::unordered_set<const Graph::Vertex*>> components_;
        std::unordered_set<const Graph::Vertex*> critical_vertices_;

        void bfs(const Graph::Vertex* vertex, BFSData& data);
        void reverse(const Graph::Vertex* vertex, BFSData& data,
                     std::unordered_map<const Graph::Vertex*, double>& delta);
        void dfs(const Graph::Vertex* vertex, std::unordered_set<const Graph::Vertex*>& visited, std::unordered_set<const Graph::Vertex*>& component);
    };
}