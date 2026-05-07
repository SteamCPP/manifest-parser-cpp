#pragma once
#include "types.hpp"
#include <future>

namespace ManifestParser {
    class ProcessZip {
        public:
            static GameData run(const std::string& path);
            static std::future<GameData> runAsync(const std::string& path);
    };
}