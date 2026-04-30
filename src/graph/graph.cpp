#include "graph.h"

Vertex* Graph::addVertex(const std::string& file_name) {
    auto vertex = std::make_unique<Vertex>(file_name);
    Vertex* ptr = vertex.get();
    vertices_[file_name] = std::move(vertex);
    return ptr;
}

Vertex* Graph::findVertex(const std::string& file_name) {
    auto it = vertices_.find(file_name);
    if (it != vertices_.end()) {
        return it->second.get();
    }
    else {
        return nullptr;
    }
}

void Graph::addEdge(Vertex* from, Vertex* to) {
    if (!from || !to) return;

    from->dependents.insert(to);
    to->dependencies.insert(from);
}
