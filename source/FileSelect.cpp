#include <dirent.h>
#include <string>

#include <FileSelect.hpp>
#include <TextReader.hpp>
#include <Log.hpp>

FileSelect::FileSelect(std::string path)
: m_path(path),
  m_failed(false)
{
    DIR* dir = opendir(path.c_str());
    if (dir) {
        Log::log("Opened dir %s", path.c_str());
        struct dirent* ent;
        std::string filename;
        while ((ent = readdir(dir))) {
            filename = ent->d_name;
            switch (ent->d_type) {
                case DT_DIR:
                    m_dirs.push_back(filename);
                    break;
                case DT_REG:
                    if (endsWith(filename, ".txt"))
                        m_textFiles.push_back(filename);
                    break;
            }
        }

        closedir(dir);

        m_dirs.sort(compareIgnoreCase);
        m_textFiles.sort(compareIgnoreCase);

    } else {
        Log::log("Could not open dir %s", path.c_str());
        m_failed = true;
    }
}
FileSelect::~FileSelect() {}

tsl::Element* FileSelect::createUI() {
    this->setTitle("Select File...");

    auto frame = new tsl::element::Frame();
    auto list = new tsl::element::List(40, 110, 7);

    for (std::string d : m_dirs) {
        auto item = new tsl::element::ListItem(d + "/");
        item->setClickListener([this, d](s64 keys){
            if (keys & KEY_A) {
                tsl::Gui::changeTo(new FileSelect(m_path + d + "/"));
                return true;
            }
            return false;
        });
        list->addItem(item);
    }

    for (std::string t : m_textFiles) {
        auto item = new tsl::element::ListItem(t);
        item->setClickListener([this, t](s64 keys){
            if (keys & KEY_A) {
                tsl::Gui::changeTo(new TextReader(m_path + t));
                return true;
            }
            return false;
        });
        list->addItem(item);
    }

    frame->addElement(list);

    if (m_failed) {
        frame->addElement(new tsl::element::CustomDrawer(0, 0, FB_WIDTH, FB_HEIGHT, [this](u16 x, u16 y, tsl::Screen *screen) {
            screen->drawString("Could not open directory", true, 20, 100, 16, tsl::a(0xFFFF));
        }));
    }

    return frame;
}

bool FileSelect::endsWith(std::string str, std::string suffix) {
    auto lstr = toLowerCase(str);
    auto lsuffix = toLowerCase(suffix);
    return lstr.size() >= lsuffix.size() &&
           lstr.compare(lstr.size()-lsuffix.size(), lsuffix.size(), lsuffix) == 0;
}

bool FileSelect::compareIgnoreCase(std::string s1, std::string s2) {
    return toLowerCase(s1) < toLowerCase(s2);
}

std::string FileSelect::toLowerCase(std::string str) {
    std::string newstr(str);
    std::transform(newstr.begin(), newstr.end(), newstr.begin(), ::tolower);
    return newstr;
}
