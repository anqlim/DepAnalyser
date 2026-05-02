#pragma once
#include "detector.h"
#include <stack>

namespace DepAnalyser::Detection {

    class CycleDetector : public Detector {
    public:
        explicit CycleDetector(const Graph::Graph& graph) : Detector(graph) {}
        void run() override;
        [[nodiscard]] const std::vector<std::unordered_set<const Graph::Vertex*>>& result() const;

    private:
        struct NodeInfo {
            int disc;
            int low;
        };
        std::unordered_map<const Graph::Vertex*, NodeInfo> node_data_;
        int index_ = 0;
        std::stack<const Graph::Vertex*> stack_;
        std::unordered_set<const Graph::Vertex*> in_stack_;
        std::vector<std::unordered_set<const Graph::Vertex*>> scc_;

        void dfs(const Graph::Vertex* vertex);
    };
}
