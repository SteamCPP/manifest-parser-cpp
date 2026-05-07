#include "lua_parser.hpp"
#include <cctype>

using namespace ManifestParser;

// Small helper that returns true if input haystack contains needle.
bool contains(std::string_view haystack, std::string_view needle) {
    return haystack.find(needle) != std::string_view::npos;
}

// Trims leading and trailing whitespace from a string_view
std::string_view trim(std::string_view v) {
    while (!v.empty() && std::isspace(static_cast<unsigned char>(v.front()))) v.remove_prefix(1);
    while (!v.empty() && std::isspace(static_cast<unsigned char>(v.back()))) v.remove_suffix(1);
    return v;
}

// Outputs arguments seperated by comma given start position and line view.
std::vector<std::string_view> get_args(size_t start,std::string_view line) {
    std::vector<std::string_view> output;

    size_t end = line.find(')',start);
    if (end == std::string_view::npos) 
        throw std::runtime_error("Could not find end parentheses for line");
    std::string_view args_sub = line.substr(start+1,end-start-1);
    while (!args_sub.empty())
    {
        size_t pos = args_sub.find(',');
        std::string_view token = args_sub.substr(0, pos);

        output.push_back(trim(token));

        if (pos == std::string_view::npos)
            break;
        args_sub.remove_prefix(pos + 1);
    }
    return output;
}

// Outputs the comment content (name) given a line view.
std::string_view get_name(std::string_view line) {
    size_t start = line.rfind("--");
    if (start == std::string_view::npos) 
        return {};
    return trim(line.substr(start+2));
}

// Outputs the lua string as a C++ string_view without quotes, given the input str with quotes.
constexpr std::string_view get_quoteless_str(std::string_view str) {
    if (str.size() >= 2 && str.front() == '"' && str.back() == '"')
        return str.substr(1, str.size() - 2);
    return str;
}

// Modifies input GameData to include structured data from a lua manifest content string view.
void ParseLua::run(GameData& input, const std::string_view& content) {
    size_t pos = 0;
    bool first_app = true;
    while (pos < content.size()) {
        size_t end = content.find('\n', pos);

        std::string_view line =
            (end == std::string_view::npos)
            ? content.substr(pos) // last line
            : content.substr(pos, end - pos); // line

        if (line.starts_with("addappid(")) {
            std::vector<std::string_view> args = get_args(8,line); // 9chars-1
            std::string_view desc = get_name(line);

            std::string name = desc.empty() ? "Unknown" : std::string(desc);
            std::string id = std::string(args[0]);

            if (first_app) {
                input.app_id = id;
                input.game_name = name;
                first_app = false;
            }
            else if (args.size() > 2) {
                input.depots[id] = {
                    .key = std::string(get_quoteless_str(args[2])),
                    .desc = name
                };
            }
            else {
                input.dlcs[id] = name;
            }
        }

        if (line.starts_with("setManifestid(")) {
            auto args = get_args(13, line); // 14chars-1

            if (args.size() >= 3) {
                input.manifest_sizes[std::string(args[0])] = std::string(args[2]); // id = size_bytes
            }
        }

        if (line.starts_with("addtoken(")) {
            auto args = get_args(8, line); // 9chars-1

            input.app_token = std::string(args[0]);
        }

        if (end == std::string_view::npos)
            break; // final line
        pos = end + 1;
    }
}