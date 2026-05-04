#include "cpp_parser.h"

namespace DepAnalyser::Parsing {

    std::vector<std::string> CppParser::parse(const std::string& path,
                                              const std::string& current_dir) {
        std::ifstream file(path);
        if (!file.is_open()) return {};

        std::string line;
        std::vector<std::string> dependencies;
        while (std::getline(file, line)) {
            if (line.find("#include") != std::string::npos) {
                size_t start = line.find('\"');
                size_t end = line.find('\"', start + 1);
                if (start != std::string::npos && end != std::string::npos) {
                    std::string file_name = line.substr(start + 1, end - start - 1);
                    std::filesystem::path resolved = std::filesystem::path(current_dir) / file_name;
                    dependencies.push_back(resolved.string());
                }
            }
        }
        return dependencies;
    }

}