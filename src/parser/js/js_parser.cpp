#include "js_parser.h"

namespace DepAnalyser::Parsing {

    std::vector<std::string> JsParser::parse(const std::string& path,
                                             const std::string& current_dir) {
        std::ifstream file(path);
        if (!file.is_open()) return {};

        std::string line;
        std::vector<std::string> dependencies;
        while (std::getline(file, line)) {
            // только относительные импорты: ./ или ../
            size_t pos = line.find("from '");
            if (pos == std::string::npos) pos = line.find("from \"");
            if (pos == std::string::npos) pos = line.find("require('");
            if (pos == std::string::npos) pos = line.find("require(\"");
            if (pos == std::string::npos) continue;

            size_t start = line.find_first_of("'\"", pos) + 1;
            size_t end = line.find_first_of("'\"", start);
            if (start == std::string::npos || end == std::string::npos) continue;

            std::string import_path = line.substr(start, end - start);
            if (import_path[0] != '.') continue; // пропускаем node_modules

            // добавляем расширение если нет
            if (import_path.find('.') == std::string::npos) {
                import_path += ".js";
            }

            std::filesystem::path resolved = std::filesystem::path(current_dir) / import_path;
            dependencies.push_back(resolved.string());
        }
        return dependencies;
    }

}