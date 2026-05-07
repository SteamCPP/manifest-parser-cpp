#include "split_manifests.hpp"

using namespace ManifestParser;
using string = std::string;

struct DepotManifest {
    string depot_id;
    string manifest_id;
};

// Splits filename into a depot id and manifest id
DepotManifest split_filename(const std::string& filename) {
    size_t us_pos = filename.find('_');
    size_t dot_pos = filename.find_last_of('.');
    if (us_pos == string::npos || dot_pos == string::npos || dot_pos <= us_pos) {
        throw std::runtime_error("Invalid filename format");
    }

    return {
        filename.substr(0, us_pos),
        filename.substr(us_pos + 1, dot_pos - us_pos - 1)
    };
}

void SplitManifests::run(GameData& input, const std::vector<string>& names) {
    for (const string& filename : names) {
        auto [depot,manifest] = split_filename(filename);
        input.manifests[depot] = manifest;
    }
}