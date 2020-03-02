#include <Utils.hpp>

#include <algorithm>

namespace utils
{
    bool endsWith(std::string const &str, std::string const &suffix, bool ignoreCase) {
        auto strIC = ignoreCase ? toLowerCase(str) : str;
        auto suffixIC = ignoreCase ? toLowerCase(suffix) : suffix;
        return strIC.size() >= suffixIC.size() &&
            strIC.compare(strIC.size() - suffixIC.size(), suffixIC.size(), suffixIC) == 0;
    }

    bool compareIgnoreCase(std::string const &s1, std::string const &s2) {
        return toLowerCase(s1) < toLowerCase(s2);
    }

    std::string toLowerCase(std::string const &str) {
        std::string newstr(str);
        std::transform(newstr.begin(), newstr.end(), newstr.begin(), ::tolower);
        return newstr;
    }
}
