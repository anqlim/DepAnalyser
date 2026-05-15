#include "critical_vertex.h"

namespace DepAnalyser::Detection {

    const std::unordered_set<const Graph::Vertex*>& CriticalVertexDetector::result() const {
        checkRan();
        return critical_vertices_;
    }

    void CriticalVertexDetector::run() {
        for (auto& vertex : graph_.getVertices()) {
            Graph::Vertex* vertex_ptr = vertex.second.get();
            BFSData data;
            std::unordered_map<const Graph::Vertex*, double> delta;

            bfs(vertex_ptr, data);
            reverse(vertex_ptr, data, delta);
        }

        std::unordered_set<const Graph::Vertex*> visited;
        for (auto& vertex : graph_.getVertices()) {
            Graph::Vertex* vertex_ptr = vertex.second.get();
            if (!visited.contains(vertex_ptr)) {
                std::unordered_set<const Graph::Vertex *> component;
                visited.insert(vertex_ptr);
                component.insert(vertex_ptr);
                dfs(vertex_ptr, visited, component);
                components_.push_back(component);
            }
        }

        for (auto& component : components_) {
            auto mx = *component.begin();
            for (auto& v : component) {
                if (centrality_[v] > centrality_[mx]) mx = v;
            }
            if (centrality_[mx] > 0) {
                critical_vertices_.insert(mx);
            }
        }

        markRan();
    }

    void CriticalVertexDetector::bfs(const Graph::Vertex* vertex, BFSData& data) {
        std::queue<const Graph::Vertex*> queue;
        queue.push(vertex);
        data.sigma[vertex] = 1;
        data.dist[vertex] = 0;
        while (!queue.empty()) {
            const Graph::Vertex* u = queue.front();
            queue.pop();
            data.stack.push(u);
            for (auto& neighbour : u->dependencies) {
                if (data.dist.find(neighbour) == data.dist.end()) {
                    data.dist[neighbour] = data.dist[u] + 1;
                    queue.push(neighbour);
                }
                if (data.dist[neighbour] == data.dist[u] + 1) {
                    data.sigma[neighbour] += data.sigma[u];
                    data.predecessors[neighbour].push_back(u);
                }
            }
        }
    }

    void CriticalVertexDetector::reverse(const Graph::Vertex* vertex, BFSData& data,
                                         std::unordered_map<const Graph::Vertex*, double>& delta) {
        while (!data.stack.empty()) {
            const Graph::Vertex* v = data.stack.top();
            data.stack.pop();
            for (auto& u : data.predecessors[v]) {
                delta[u] += (static_cast<double>(data.sigma[u]) / static_cast<double>(data.sigma[v])) * (1 + delta[v]);
            }
            if (v != vertex) {
                centrality_[v] += delta[v];
            }
        }
    }

    void CriticalVertexDetector::dfs(const Graph::Vertex* vertex, std::unordered_set<const Graph::Vertex*>& visited, std::unordered_set<const Graph::Vertex*>& component) {
        for (auto& v : vertex->dependencies) {
            if (!component.contains(v)) {
                visited.insert(v);
                component.insert(v);
                dfs(v, visited, component);
            }
        }
        for (auto& v : vertex->dependents) {
            if (!component.contains(v)) {
                visited.insert(v);
                component.insert(v);
                dfs(v, visited, component);
            }
        }
    }

}