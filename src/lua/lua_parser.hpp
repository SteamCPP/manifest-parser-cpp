#pragma once
#include "manifestparser/types.hpp"
#include <vector>

namespace ManifestParser {
    struct GameData;
    
    class ParseLua {
        public:
            static void run(GameData& input, const std::string_view& content);
    };
}