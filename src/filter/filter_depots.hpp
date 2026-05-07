#pragma once
#include "manifestparser/types.hpp"
#include <array>
#include <algorithm>

namespace ManifestParser {
    struct GameData;
    constexpr std::array<size_t, 31> DEPOT_BLACKLIST = {
        228981,
        228982,
        228983,
        228984,
        228985,
        228986,
        228987,
        228988,
        228989,
        229000,
        229001,
        229002,
        229003,
        229004,
        229005,
        229006,
        229007,
        229010,
        229011,
        229012,
        229020,
        229030,
        229031,
        229032,
        229033,
        228990,
        239142,
        798541,
        798542,
        798543,
        1034630
    };

    class FilterDepots {
        public:
            static void run(GameData& input);
    };
}