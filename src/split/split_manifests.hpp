#pragma once
#include "manifestparser/types.hpp"
#include <string>
#include <vector>

namespace ManifestParser {
    struct GameData;
    
    class SplitManifests {
        public:
            static void run(GameData& input, const std::vector<std::string>& names);
    };
}