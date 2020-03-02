#pragma once

#include <json.hpp>
#include <functional>

using nlohmann::json;

class Config {
public:
    static json read();
    static void update(std::function<void(json &)> const &updater);

private:
    static json initialize();

private:
    Config() {}
    static const char* FILE_PATH;
};
