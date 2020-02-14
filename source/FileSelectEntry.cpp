#include <FileSelectEntry.hpp>
#include <FileSelect.hpp>
#include <TextReader.hpp>
#include <Config.hpp>

namespace fsel
{
    FileSelectEntry::FileSelectEntry(std::string path, bool favorite)
    : m_path(path),
      m_favorite(favorite)
    {}
    FileSelectEntry::~FileSelectEntry() {}

    std::string FileSelectEntry::getName() {
        int start = 0;
        for (int i = getPath().length()-2; i >= 0; i--) {
            if (getPath().at(i) == '/') {
                start = i + 1;
                break;
            }
        }
        return getPath().substr(start);
    }

    void FileSelectEntry::favorite() {
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

    FileSelectDirEntry::FileSelectDirEntry(std::string path, bool favorite)
    : FileSelectEntry(path, favorite)
    {}
    FileSelectDirEntry::~FileSelectDirEntry() {}

    void FileSelectDirEntry::select() {
        tsl::Gui::changeTo(new FileSelect(getPath()));
    }

    FileSelectFileEntry::FileSelectFileEntry(std::string path, bool favorite)
    : FileSelectEntry(path, favorite)
    {}
    FileSelectFileEntry::~FileSelectFileEntry() {}

    void FileSelectFileEntry::select() {
        tsl::Gui::changeTo(new TextReader(getPath()));
    }

    bool compareIgnoreCase(std::string s1, std::string s2) {
        return toLowerCase(s1) < toLowerCase(s2);
    }

    std::string toLowerCase(std::string str) {
        std::string newstr(str);
        std::transform(newstr.begin(), newstr.end(), newstr.begin(), ::tolower);
        return newstr;
    }
}
