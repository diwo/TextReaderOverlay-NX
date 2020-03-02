#pragma once

#include <string>

namespace utils
{
    extern bool endsWith(std::string const &str, std::string const &suffix, bool ignoreCase = false);
    extern bool compareIgnoreCase(std::string const &s1, std::string const &s2);
    extern std::string toLowerCase(std::string const &str);
}
