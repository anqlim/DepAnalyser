#include "python_parser.h"

namespace DepAnalyser::Parsing {

    std::vector<std::string> PythonParser::parse(const std::string& path,
                                                 const std::string& current_dir) {
        std::ifstream file(path);
        if (!file.is_open()) return {};

        std::string line;
        std::vector<std::string> dependencies;
        while (std::getline(file, line)) {
            // from .module import something
            // import .module
            size_t dot = line.find('.');
            if (dot == std::string::npos) continue;

            // берём имя модуля после точки до пробела
            size_t end = line.find(' ', dot + 1);
            std::string module = line.substr(dot + 1, end - dot - 1);
            if (module.empty()) continue;

            std::filesystem::path resolved = std::filesystem::path(current_dir) / (module + ".py");
            dependencies.push_back(resolved.string());
        }
        return dependencies;
    }

}