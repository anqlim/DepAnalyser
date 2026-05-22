#pragma once
#include "../parser.h"
#include <fstream>

namespace DepAnalyser::Parsing {

    class JsParser : public Parser {
    public:
        std::vector<std::string> parse(const std::string& path,
                                                 const std::string& current_dir,
                                                 const std::string& project_root) override;
    };

}
