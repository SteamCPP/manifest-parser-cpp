#include "filter_depots.hpp"
#include <string>

using namespace ManifestParser;

void FilterDepots::run(GameData& input) {
    for (int id : DEPOT_BLACKLIST) {
        input.depots.erase(std::to_string(id));
    }
}