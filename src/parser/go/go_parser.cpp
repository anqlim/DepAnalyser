#include "go_parser.h"

namespace DepAnalyser::Parsing {

    static std::string readModuleName(const std::string& project_root) {
        std::ifstream mod(std::filesystem::path(project_root) / "go.mod");
        if (!mod.is_open()) return "";
        std::string line;
        while (std::getline(mod, line)) {
            if (line.rfind("module ", 0) == 0) {
                return line.substr(7);
            }
        }
        return "";
    }

    std::vector<std::string> GoParser::parse(const std::string& path,
                                             const std::string& current_dir,
                                             const std::string& project_root) {
        std::ifstream file(path);
        if (!file.is_open()) return {};

        std::string module_name = readModuleName(project_root);

        std::string line;
        std::vector<std::string> dependencies;
        bool in_block = false;

        while (std::getline(file, line)) {
            if (line.find("import (") != std::string::npos) { in_block = true; continue; }
            if (in_block && line.find(')') != std::string::npos) { in_block = false; continue; }

            if (line.find("import \"") != std::string::npos || in_block) {
                size_t s = line.find('"'), e = line.find('"', s + 1);
                if (s == std::string::npos || e == std::string::npos) continue;

                std::string import_path = line.substr(s + 1, e - s - 1);

                if (module_name.empty() || import_path.rfind(module_name, 0) != 0) continue;

                std::string rel = import_path.substr(module_name.size());
                if (!rel.empty() && rel[0] == '/') rel = rel.substr(1);

                auto pkg_dir = (std::filesystem::path(project_root) / rel).lexically_normal();
                if (!std::filesystem::exists(pkg_dir) || !std::filesystem::is_directory(pkg_dir)) continue;

                for (auto& entry : std::filesystem::directory_iterator(pkg_dir)) {
                    if (entry.path().extension() == ".go") {
                        dependencies.push_back(entry.path().string());
                    }
                }
            }
        }
        return dependencies;
    }

}