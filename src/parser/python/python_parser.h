#pragma once
#include "../parser.h"
#include <fstream>

namespace DepAnalyser::Parsing {

    class PythonParser : public Parser{
    public:
        std::vector<std::string> parse(const std::string& path,
                                       const std::string& current_dir) override;
    };

}