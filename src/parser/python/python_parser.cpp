#include "python_parser.h"

namespace DepAnalyser::Parsing {

    std::vector<std::string> PythonParser::parse(const std::string& path,
                                                 const std::string& current_dir,
                                                 const std::string& project_root) {
        std::ifstream file(path);
        if (!file.is_open()) return {};

        auto tryResolve = [&](const std::string& module) -> std::string {
            auto local = (std::filesystem::path(current_dir) / (module + ".py")).lexically_normal();
            if (std::filesystem::exists(local)) return local.string();
            auto root = (std::filesystem::path(project_root) / (module + ".py")).lexically_normal();
            if (std::filesystem::exists(root)) return root.string();
            return "";
        };

        std::string line;
        std::vector<std::string> dependencies;
        while (std::getline(file, line)) {
            size_t pos = line.find_first_not_of(" \t");
            if (pos == std::string::npos) continue;
            std::string t = line.substr(pos);

            std::string module;
            if (t.rfind("from .", 0) == 0) {
                size_t dot = t.find('.');
                size_t space = t.find(' ', dot + 1);
                if (space == std::string::npos) continue;
                module = t.substr(dot + 1, space - dot - 1);
            } else if (t.rfind("from ", 0) == 0) {
                size_t space = t.find(' ', 5);
                module = t.substr(5, space == std::string::npos ? std::string::npos : space - 5);
            } else if (t.rfind("import ", 0) == 0) {
                module = t.substr(7);
                module = module.substr(0, module.find_first_of(" \n\r"));
            } else continue;

            if (module.empty()) continue;
            auto resolved = tryResolve(module);
            if (!resolved.empty()) dependencies.push_back(resolved);
        }
        return dependencies;
    }

}