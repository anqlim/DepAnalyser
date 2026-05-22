#include "js_parser.h"

namespace DepAnalyser::Parsing {

    std::vector<std::string> JsParser::parse(const std::string& path,
                                             const std::string& current_dir,
                                             const std::string& project_root) {
        std::ifstream file(path);
        if (!file.is_open()) return {};

        auto tryResolve = [&](const std::string& import_path) -> std::string {
            auto base = import_path[0] == '.'
                        ? (std::filesystem::path(current_dir) / import_path).lexically_normal()
                        : (std::filesystem::path(project_root) / import_path).lexically_normal();

            if (std::filesystem::exists(base)) return base.string();
            for (auto& ext : {".js", ".ts"}) {
                auto with_ext = std::filesystem::path(base.string() + ext);
                if (std::filesystem::exists(with_ext)) return with_ext.string();
            }
            return "";
        };

        std::string line;
        std::vector<std::string> dependencies;
        while (std::getline(file, line)) {
            size_t pos = line.find("from '");
            if (pos == std::string::npos) pos = line.find("from \"");
            if (pos == std::string::npos) pos = line.find("require('");
            if (pos == std::string::npos) pos = line.find("require(\"");
            if (pos == std::string::npos) continue;

            size_t s = line.find_first_of("'\"", pos) + 1;
            size_t e = line.find_first_of("'\"", s);
            if (s == std::string::npos || e == std::string::npos) continue;

            auto resolved = tryResolve(line.substr(s, e - s));
            if (!resolved.empty()) dependencies.push_back(resolved);
        }
        return dependencies;
    }

}