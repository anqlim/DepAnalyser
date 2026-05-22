#pragma once
#include "../parser.h"
#include <fstream>
#include <algorithm>

namespace DepAnalyser::Parsing {

    class JavaParser : public Parser {
    public:
        std::vector<std::string> parse(const std::string& path,
                                                   const std::string& current_dir,
                                                   const std::string& project_root) override;
    };

}
