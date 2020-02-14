#include <tesla.hpp>

#include <FavoritesMenu.hpp>
#include <FileSelectEntry.hpp>
#include <Config.hpp>

FavoritesMenu::FavoritesMenu() {
    auto favorites = Config::read()["favorites"];
    if (!favorites.is_array()) {
        favorites = {};
    }
    for (auto &b : favorites) {
        std::string path = b;
        if (path[path.length() -1] == '/') {
            m_entries.push_back(new FileSelectDirEntry(path, true));
        } else {
            m_entries.push_back(new FileSelectFileEntry(path, true));
        }
    }
    m_entries.sort([](FileSelectEntry *a, FileSelectEntry *b) { return *a < *b; });
}
FavoritesMenu::~FavoritesMenu() {
    for (auto it = m_entries.begin(); it != m_entries.end(); it++) {
        delete (*it);
    }
}

tsl::Element* FavoritesMenu::createUI() {
    auto *frame = new tsl::element::Frame();

    if (m_entries.empty()) {
        frame->addElement(new tsl::element::CustomDrawer(0, 0, FB_WIDTH, FB_HEIGHT, [this](u16 x, u16 y, tsl::Screen *screen) {
            screen->drawString("No favorites added yet", true, 20, 100, 16, tsl::a(0xFFFF));
        }));
    }

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

void FavoritesMenu::preDraw(tsl::Screen *screen) {
    screen->fillScreen(tsl::a({ 0x0, 0x0, 0x0, 0xD }));
    screen->drawString("Favorites", false, 20, 50, 30, tsl::a(0xFFFF));
    screen->drawRect(15, 720 - 73, FB_WIDTH - 30, 1, tsl::a(0xFFFF));
    screen->drawString("\uE0E3  Favorite", false, 30, 693, 23, tsl::a(0xFFFF));
}
