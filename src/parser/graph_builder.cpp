#include "graph_builder.h"
#include <iostream>

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

        const std::unordered_set<std::string> ignored_dirs = {
                "build", "cmake-build-debug", "cmake-build-release", ".git", "node_modules", "_deps", ".cache"
        };

        for (auto it = std::filesystem::recursive_directory_iterator(
                project_path_, std::filesystem::directory_options::skip_permission_denied);
             it != std::filesystem::recursive_directory_iterator(); ++it) {
            if (it->is_directory() && ignored_dirs.contains(it->path().filename().string())) {
                it.disable_recursion_pending();
                continue;
            }
            if (it->is_regular_file()) {
                std::string ext = it->path().extension().string();
                if (supported.contains(ext)) {
                    files.push_back(it->path().string());
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
        return { path, parser->parse(path, current_dir, project_path_) };
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