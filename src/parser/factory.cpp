#include "factory.h"

namespace DepAnalyser::Parsing {

    std::unique_ptr<Parser> ParserFactory::create(std::string_view extension) {
        if (extension == ".cpp" || extension == ".h" ||
            extension == ".hpp" || extension == ".c") {
            return std::make_unique<CppParser>();
        }
        if (extension == ".py") {
            return std::make_unique<PythonParser>();
        }
        if (extension == ".java") {
            return std::make_unique<JavaParser>();
        }
        if (extension == ".js" || extension == ".ts") {
            return std::make_unique<JsParser>();
        }
        if (extension == ".go") {
            return std::make_unique<GoParser>();
        }
        return nullptr;
    }

}