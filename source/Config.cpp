#include <fstream>

#include <Config.hpp>

const char* Config::FILE_PATH = "sdmc:/switch/.overlays/TextReaderOverlay/config.json";

json Config::read() {
    std::fstream fs;
    json j;

    fs.open(FILE_PATH, std::fstream::in);
    if (fs.fail()) {
        j = initialize();
    } else {
        j = json::parse(fs, nullptr, false);
        fs.close();
        if (j.is_discarded()) {
            j = initialize();
        }
    }

    return j;
}

void Config::update(std::function<void(json &)> updater) {
    std::fstream fs;
    json j = read();

    updater(j);

    fs.open(FILE_PATH, std::fstream::out);
    fs << j.dump(2);
    fs.close();
}

json Config::initialize() {
    std::fstream fs(FILE_PATH, std::fstream::out | std::fstream::trunc);
    json j = json::object();
    fs << j;
    fs.close();
    return j;
}
