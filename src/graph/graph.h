#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>

namespace DepAnalyser::Graph {

    struct Vertex {
        explicit Vertex(std::string file_name) : file_name(std::move(file_name)) {}
        std::string file_name;
        std::unordered_set<Vertex*> dependencies;
        std::unordered_set<Vertex*> dependents;
    };

    class Graph {
    private:
        std::unordered_map<std::string, std::unique_ptr<Vertex>> vertices_;

    public:
        Vertex* addVertex(const std::string& file_name);
        Vertex* findVertex(const std::string& file_name);
        void addEdge(Vertex* from, Vertex* to);
        [[nodiscard]] const std::unordered_map<std::string, std::unique_ptr<Vertex>>& getVertices() const;
    };
}

