#pragma once
#include <string>
#include <optional>
#include <cstdint>
#include <unordered_map>
#include <stdexcept>

namespace ManifestParser {
    struct DepotData {
        std::string key;
        std::string desc;

        std::optional<uint64_t> size;
        std::optional<std::string> oslist;
        std::optional<std::string> language;

    };

    struct GameData {
        std::string app_id;
        std::string build_id;

        std::string game_name;
        std::string app_token;
        std::string header_url;
        std::string install_dir;

        std::unordered_map<std::string, DepotData> depots;
        std::unordered_map<std::string, std::string> dlcs;
        std::unordered_map<std::string, std::string> manifests;
        std::unordered_map<std::string, std::string> manifest_sizes;
    };
}