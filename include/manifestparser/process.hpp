#pragma once
#include "types.hpp"

namespace ManifestParser {
    class ProcessZip {
        public:
            static GameData run(const std::string& path);
    };
}