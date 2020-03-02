#include <FileSelectEntry.hpp>

#include <Config.hpp>

void FileSelectEntry::toggleFavorite() {
    m_favorite = !m_favorite;
    Config::update([this](json &j) {
        if (!j["favorites"].is_array()) {
            j["favorites"] = {};
        }
        if (m_favorite) {
            j["favorites"].push_back(getPath());
        } else {
            auto found = std::find(j["favorites"].begin(), j["favorites"].end(), getPath());
            if (found != j["favorites"].end()) {
                j["favorites"].erase(found);
            }
        }
    });
}

bool FileSelectEntry::operator<(FileSelectEntry const &other) const {
    if (ordering() < other.ordering())
        return true;
    else if (other.ordering() < ordering())
        return false;
    else
        return utils::compareIgnoreCase(m_path, other.m_path);
}
