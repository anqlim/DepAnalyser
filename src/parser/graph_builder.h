#pragma once
#include "parser.h"
#include "factory.h"
#include "../common/tread_pool/thread_pool.h"

namespace DepAnalyser::Parsing {

    class GraphBuilder {
    public:
        explicit GraphBuilder(std::string_view project_path) : project_path_(project_path) {}
        Graph::Graph build();

    private:
        std::string project_path_;
        Graph::Graph graph_;

        [[nodiscard]] std::vector<std::string> collectFiles() const;
        [[nodiscard]] Types::ParseResult parseFile(const std::string& path) const;
        void buildGraph(const std::vector<Types::ParseResult>& results);
    };

}
