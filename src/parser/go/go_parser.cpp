#include "go_parser.h"

namespace DepAnalyser::Parsing {

    std::vector<std::string> GoParser::parse(const std::string& path,
                                             const std::string& current_dir) {
        std::ifstream file(path);
        if (!file.is_open()) return {};

        std::string line;
        std::vector<std::string> dependencies;
        bool in_import_block = false;

        while (std::getline(file, line)) {
            // блочный импорт: import ( ... )
            if (line.find("import (") != std::string::npos) {
                in_import_block = true;
                continue;
            }
            if (in_import_block && line.find(')') != std::string::npos) {
                in_import_block = false;
                continue;
            }

            // одиночный импорт: import "path"
            // или строка внутри блока
            if (line.find("import \"") != std::string::npos || in_import_block) {
                size_t start = line.find('\"');
                size_t end = line.find('\"', start + 1);
                if (start == std::string::npos || end == std::string::npos) continue;

                std::string import_path = line.substr(start + 1, end - start - 1);
                if (import_path[0] != '.') continue; // пропускаем стандартную библиотеку

                std::filesystem::path resolved = std::filesystem::path(current_dir) / import_path;
                dependencies.push_back(resolved.string());
            }
        }
        return dependencies;
    }

}