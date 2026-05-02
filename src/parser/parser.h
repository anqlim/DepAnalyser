#pragma once
#include "../graph/graph.h"
#include <string_view>
#include <mutex>
#include <filesystem>

namespace DepAnalyser::Parsing {

    class Parser {
    public:
        virtual ~Parser() = default;
        virtual std::vector<std::string> parse(const std::string& path,
                                               const std::string& current_dir) = 0;
    };

}