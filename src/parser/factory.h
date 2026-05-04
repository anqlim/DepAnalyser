#pragma once
#include "parser.h"
#include "cpp/cpp_parser.h"
#include "python/python_parser.h"
#include "java/java_parser.h"
#include "js/js_parser.h"
#include "go/go_parser.h"

namespace DepAnalyser::Parsing {

    class ParserFactory {
    public:
        static std::unique_ptr<Parser> create(std::string_view extension);
    };

}
