#pragma once
#include "parser.h"
#include "factory.h"

namespace DepAnalyser::Parsing {

    class GraphBuilder {
    public:
        explicit GraphBuilder(std::string_view project_path) : project_path_(project_path) {}
        Graph::Graph build();

    private:
        std::string project_path_;
        Graph::Graph graph_;

        struct ParseResult {
            std::string file_path;
            std::vector<std::string> dependencies;
        };

        std::vector<std::string> collectFiles() const;
        ParseResult parseFile(const std::string& path) const;
        void buildGraph(const std::vector<ParseResult>& results);
    };

}
