#include "cycle.h"

namespace DepAnalyser::Detection {

    const std::vector<std::unordered_set<const Graph::Vertex*>>& CycleDetector::result() const {
        checkRan();
        return scc_;
    }

    void CycleDetector::run() {
        for (auto& vertex : graph_.getVertices()) {
            Graph::Vertex* vertex_ptr = vertex.second.get();
            if (node_data_.find(vertex_ptr) == node_data_.end()) {
                dfs(vertex_ptr);
            }
        }

        markRan();
    }

    void CycleDetector::dfs(const Graph::Vertex* vertex) {
        node_data_[vertex] = {index_, index_};
        index_++;
        stack_.push(vertex);
        in_stack_.insert(vertex);
        for (auto neighbour : vertex->dependencies) {
            if (node_data_.find(neighbour) == node_data_.end()) {
                dfs(neighbour);
                node_data_[vertex].low = std::min(node_data_[vertex].low, node_data_[neighbour].low);
            }
            else if (in_stack_.contains(neighbour)) {
                node_data_[vertex].low = std::min(node_data_[vertex].low, node_data_[neighbour].disc);
            }
        }

        if (node_data_[vertex].low == node_data_[vertex].disc) {
            std::unordered_set<const Graph::Vertex*> new_scc;
            while (stack_.top() != vertex) {
                auto top = stack_.top();
                stack_.pop();
                in_stack_.erase(top);
                new_scc.insert(top);
            }
            stack_.pop();
            in_stack_.erase(vertex);
            new_scc.insert(vertex);
            if (new_scc.size() > 1) scc_.push_back(new_scc);
        }
    }
}
