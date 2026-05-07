#include "manifestparser/process.hpp"

#include "filter/filter_depots.hpp"
#include "lua/lua_parser.hpp"
#include "split/split_manifests.hpp"

extern "C" {
#include <miniz/miniz.h>
}

using namespace ManifestParser;
using string = std::string;

// Returns the content of a file from an open zip archive object and filename char array.
string read_file_from_open_zip(mz_zip_archive& zip, const char* filename) {
    size_t size = 0;
    void* data = mz_zip_reader_extract_file_to_heap(&zip, filename, &size, 0);
    if (!data)
        throw std::runtime_error("File not found in zip; invalid?");

    string result((char*)data, size);
    mz_free(data);
    return result;
}

// Finds multiple files of the same extension in the archive given an open zip archive object and extension (with period).
std::vector<string> find_all_open_zip(mz_zip_archive& zip, const string ext) {
    int file_count = (int)mz_zip_reader_get_num_files(&zip);
    std::vector<string> files;

    for (int i = 0; i < file_count; ++i) {
        mz_zip_archive_file_stat file_stat;
        if (!mz_zip_reader_file_stat(&zip, i, &file_stat))
            continue;

        string name(file_stat.m_filename);
        if (name.size() >= ext.size() && name.substr(name.size() - ext.size()) == ext) {
            files.push_back(name);
        }
    }

    return files;
}

// Returns the name of the first found file in the archive given an open zip archive object and extension (with period).
string find_in_open_zip(mz_zip_archive& zip, const string ext) {
    std::vector<string> mult = find_all_open_zip(zip,ext);
    if (mult.size() == 0) throw std::runtime_error("File not found in zip");
    return mult[0];
}

// Returns the contents of the found file from the input archive path and extension (with period).
string get_file_contents(mz_zip_archive& zip, const string ext) {
    string filename = find_in_open_zip(zip,ext);
    string content = read_file_from_open_zip(zip, filename.c_str());

    return content;
}

// Opens the zip file from a path and reference to an empty zip archive; Returns the result of the zip opening.
bool open_zip(const string& path, mz_zip_archive& zip) {
    if (!mz_zip_reader_init_file(&zip, path.c_str(), 0))
        return false;
    return true;
}

// Processes a zip and returns structured GameData.
GameData ProcessZip::run(const string& path) {
    mz_zip_archive zip{};
    if (!open_zip(path,zip)) {
        throw std::runtime_error("Failure to open zip; bad file?");
    }

    GameData output;
    string lua_content = get_file_contents(zip, ".lua");
    std::vector<string> manifests = find_all_open_zip(zip,".manifest");
    mz_zip_reader_end(&zip); // We've gotten what we need; we can close the zip reader.

    SplitManifests::run(output,manifests);
    ParseLua::run(output,lua_content); // Fills: .app_id, .game_name, .app_token, .depots, .dlcs, .manifest_sizes
    FilterDepots::run(output); // Filters the depots in place
    
    return output;
}

// Async version of the run function.
std::future<GameData> ProcessZip::runAsync(const std::string& path) {
    return std::async(std::launch::async, &ProcessZip::run, path);
}