#include <Config.hpp>

#include <fstream>

const char* Config::FILE_PATH = "sdmc:/switch/.overlays/TextReaderOverlay/config.json";

json Config::read() {
    std::fstream fs(FILE_PATH, std::fstream::in);
    if (fs.fail()) {
        return initialize();
    }

    json j = json::parse(fs, nullptr, false);
    if (!j.is_discarded())
        return j;

    return initialize();
}

void Config::update(std::function<void(json &)> const &updater) {
    json j = read();

    updater(j);

    std::fstream fs(FILE_PATH, std::fstream::out);
    fs << j.dump(2);
}

json Config::initialize() {
    std::fstream fs(FILE_PATH, std::fstream::out | std::fstream::trunc);
    json j = json::object();
    fs << j;
    return j;
}
