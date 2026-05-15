#include "python_parser.h"

namespace DepAnalyser::Parsing {

    std::vector<std::string> PythonParser::parse(const std::string& path,
                                                 const std::string& current_dir) {
        std::ifstream file(path);
        if (!file.is_open()) return {};

        std::string line;
        std::vector<std::string> dependencies;
        while (std::getline(file, line)) {
            size_t from_pos = line.find("from");
            if (from_pos == std::string::npos) continue;

            size_t dot = line.find('.', from_pos + 4);
            if (dot == std::string::npos) continue;

            std::string between = line.substr(from_pos + 4, dot - from_pos - 4);
            if (between.find_first_not_of(' ') != std::string::npos) continue;

            size_t end = line.find(' ', dot + 1);
            std::string module = line.substr(dot + 1, end - dot - 1);
            if (module.empty()) continue;

            std::filesystem::path resolved = std::filesystem::path(current_dir) / (module + ".py");
            dependencies.push_back(resolved.string());
        }
        return dependencies;
    }

}