#include "java_parser.h"

namespace DepAnalyser::Parsing {

    std::vector<std::string> JavaParser::parse(const std::string& path,
                                               const std::string& current_dir,
                                               const std::string& project_root) {
        std::ifstream file(path);
        if (!file.is_open()) return {};

        std::string line;
        std::vector<std::string> dependencies;
        while (std::getline(file, line)) {
            if (line.find("import ") == std::string::npos) continue;
            size_t start = line.find("import ") + 7;
            size_t end = line.find(';', start);
            if (end == std::string::npos) continue;

            std::string import_path = line.substr(start, end - start);
            if (import_path.starts_with("java.") || import_path.starts_with("javax.")) continue;

            std::replace(import_path.begin(), import_path.end(), '.', '/');
            auto resolved = (std::filesystem::path(project_root) / (import_path + ".java")).lexically_normal();
            if (std::filesystem::exists(resolved)) dependencies.push_back(resolved.string());
        }
        return dependencies;
    }

}