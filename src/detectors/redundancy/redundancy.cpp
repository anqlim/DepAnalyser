#include "redundancy.h"

namespace DepAnalyser::Detection {

    RedundancyDetector::RedundancyDetector(const Graph::Graph& graph,
                                const std::vector<std::unordered_set<const Graph::Vertex*>>& scc) :
                                Detector(graph), scc_(scc) {}

    const std::vector<Types::Edge>& RedundancyDetector::result() const {
        checkRan();
        return redundancy_edges_;
    }

    void RedundancyDetector::run() {
        mappingSCC();
        for (auto& vertex : graph_.getVertices()) {
            Graph::Vertex* vertex_ptr = vertex.second.get();
            for (auto& dependence : vertex_ptr->dependencies) {
                if (scc_index_.at(vertex_ptr) != scc_index_.at(dependence)) {
                    if (bfs(vertex_ptr, dependence)) {
                        redundancy_edges_.push_back({vertex_ptr, dependence});
                    }
                }
            }
        }

        markRan();
    }

    bool RedundancyDetector::bfs(const Graph::Vertex* start, const Graph::Vertex* target) {
        std::queue<const Graph::Vertex*> queue;
        queue.push(start);
        std::unordered_set<const Graph::Vertex*> visited;
        visited.insert(start);
        while (!queue.empty()) {
            const Graph::Vertex* u = queue.front();
            queue.pop();
            for (auto& neighbour : u->dependencies) {
                if (u == start && neighbour == target) continue;
                if (neighbour == target) return true;
                if (!visited.contains(neighbour)) {
                    visited.insert(neighbour);
                    queue.push(neighbour);
                }
            }
        }
        return false;
    }

    void RedundancyDetector::mappingSCC() {
        int i = 0;
        for (i = 0; i < scc_.size(); i++) {
            for (auto v : scc_[i]) {
                scc_index_[v] = i;
            }
        }
        for (auto& vertex : graph_.getVertices()) {
            Graph::Vertex* vertex_ptr = vertex.second.get();
            if (scc_index_.find(vertex_ptr) == scc_index_.end()) {
                scc_index_[vertex_ptr] = i++;
            }
        }
    }
}