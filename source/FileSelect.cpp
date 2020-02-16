#include <dirent.h>

#include <FileSelect.hpp>
#include <Config.hpp>
#include <Log.hpp>

namespace fsel
{
    FileSelect::FileSelect(std::string path)
    : m_path(path),
    m_failed(false)
    {
        DIR* dir = opendir(path.c_str());
        if (dir) {
            auto favorites = Config::read()["favorites"];
            if (!favorites.is_array()) {
                favorites = {};
            }
            auto isFavorite = [favorites](std::string path) {
                auto found = std::find(favorites.begin(), favorites.end(), path);
                return found != favorites.end();
            };
            struct dirent* ent;
            std::string filename;
            while ((ent = readdir(dir))) {
                filename = ent->d_name;
                switch (ent->d_type) {
                    case DT_DIR:
                        filename = filename + "/";
                        m_entries.push_back(new FileSelectDirEntry(path + filename, isFavorite(path + filename)));
                        break;
                    case DT_REG:
                        if (endsWith(filename, ".txt"))
                            m_entries.push_back(new FileSelectFileEntry(path + filename, isFavorite(path + filename)));
                        break;
                }
            }
            closedir(dir);
            m_entries.sort([](FileSelectEntry *a, FileSelectEntry *b) { return *a < *b; });
        }
        else {
            Log::log("Could not open dir %s", path.c_str());
            m_failed = true;
        }
    }
    FileSelect::~FileSelect() {
        for (auto it = m_entries.begin(); it != m_entries.end(); it++) {
            delete (*it);
        }
    }

    tsl::Element* FileSelect::createUI() {
        auto frame = new tsl::element::Frame();

        frame->addElement(new tsl::element::CustomDrawer(0, 0, FB_WIDTH, FB_HEIGHT, [this](u16 x, u16 y, tsl::Screen *screen) {
            if (m_failed)
                screen->drawString("Could not open directory", true, 20, 100, 16, tsl::a(0xFFFF));
            else
                screen->drawString(m_path.c_str(), true, 20, 80, 12, tsl::a(0xFFFF));
        }));

        auto list = new tsl::element::List(40, 110, 7);

        for (auto it = m_entries.begin(); it != m_entries.end(); it++) {
            auto item = new tsl::element::ListItem((*it)->label());
            item->setClickListener([it, item](s64 keys) {
                if (keys & KEY_A) {
                    (*it)->select();
                    return true;
                }
                if (keys & KEY_Y) {
                    (*it)->favorite();
                    item->updateText((*it)->label());
                    return true;
                }
                return false;
            });
            list->addItem(item);
        }

        frame->addElement(list);

        return frame;
    }

    void FileSelect::preDraw(tsl::Screen *screen) {
        screen->fillScreen(tsl::a({ 0x0, 0x0, 0x0, 0xD }));
        screen->drawString("Select File...", false, 20, 50, 30, tsl::a(0xFFFF));
        screen->drawRect(15, 720 - 73, FB_WIDTH - 30, 1, tsl::a(0xFFFF));
        screen->drawString("\uE0E3  Favorite", false, 30, 693, 23, tsl::a(0xFFFF));
    }

    bool endsWith(std::string str, std::string suffix) {
        auto lstr = toLowerCase(str);
        auto lsuffix = toLowerCase(suffix);
        return lstr.size() >= lsuffix.size() &&
            lstr.compare(lstr.size()-lsuffix.size(), lsuffix.size(), lsuffix) == 0;
    }
}
