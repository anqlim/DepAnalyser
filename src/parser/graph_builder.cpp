#include "graph_builder.h"

namespace DepAnalyser::Parsing {

    Graph::Graph GraphBuilder::build() {
        auto files = collectFiles();
        Multithreading::ThreadPool pool(std::thread::hardware_concurrency());

        std::vector<std::future<Types::ParseResult>> futures;
        for (auto& file : files) {
            futures.push_back(pool.submit([this, file] {
                return parseFile(file);
            }));
        }

        std::vector<Types::ParseResult> results;
        for (auto& future : futures) {
            results.push_back(future.get());
        }

        buildGraph(results);
        return std::move(graph_);
    }

    std::vector<std::string> GraphBuilder::collectFiles() const {
        std::vector<std::string> files;

        const std::unordered_set<std::string> supported = {
                ".c", ".cpp", ".h", ".hpp", ".py", ".java", ".js", ".ts", ".go"
        };

        for (auto& entry : std::filesystem::recursive_directory_iterator(project_path_)) {
            if (entry.is_regular_file()) {
                std::string ext = entry.path().extension().string();
                if (supported.contains(ext)) {
                    files.push_back(entry.path().string());
                }
            }
        }

        return files;
    }

    Types::ParseResult GraphBuilder::parseFile(const std::string& path) const {
        std::string extension = std::filesystem::path(path).extension().string();
        auto parser = ParserFactory::create(extension);
        if (!parser) return {path, {}};
        std::string current_dir = std::filesystem::path(path).parent_path().string();
        return { path, parser->parse(path, current_dir) };
    }

    void GraphBuilder::buildGraph(const std::vector<Types::ParseResult>& results) {
        for (auto& file : results) {
            graph_.addVertex(file.file_path);
        }
        for (auto& file : results) {
            auto* from = graph_.findVertex(file.file_path);
            for (auto& dependence : file.dependencies) {
                auto* to = graph_.findVertex(dependence);
                if (to != nullptr) graph_.addEdge(from, to);
            }
        }
    }

}