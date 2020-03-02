#include <MainMenu.hpp>

#include <Overlay.hpp>
#include <FileSelect.hpp>
#include <FavoritesMenu.hpp>
#include <HelpMenu.hpp>
#include <LogMenu.hpp>

tsl::elm::Element* MainMenu::createUI() {
    auto frame = new tsl::elm::OverlayFrame("Text Reader", TEXT_READER_VERSION);
    m_menuItems = new tsl::elm::List();

    auto favorites = new tsl::elm::ListItem("Favorites");
    favorites->setClickListener([](s64 keys) {
        if (keys & KEY_A) {
            tsl::changeTo<FavoritesMenu>();
            return true;
        }
        return false;
    });
    m_menuItems->addItem(favorites);

    auto fileSelect = new tsl::elm::ListItem("Browse...");
    fileSelect->setClickListener([](s64 keys) {
        if (keys & KEY_A) {
            tsl::changeTo<FileSelect>("sdmc:/");
            return true;
        }
        return false;
    });
    m_menuItems->addItem(fileSelect);

    auto help = new tsl::elm::ListItem("Help");
    help->setClickListener([](s64 keys) {
        if (keys & KEY_A) {
            tsl::changeTo<HelpMenu>();
            return true;
        }
        return false;
    });
    m_menuItems->addItem(help);

    frame->setContent(m_menuItems);

    return frame;
}

bool MainMenu::handleInput(u64 keysDown, u64 keysHeld, touchPosition touchInput, JoystickPosition leftJoyStick, JoystickPosition rightJoyStick) {
    if (!m_debug && (keysDown & KEY_MINUS)) {
        auto logMenu = new tsl::elm::ListItem("Show Logs");
        logMenu->setClickListener([](s64 keys) {
            if (keys & KEY_A) {
                tsl::changeTo<LogMenu>();
                return true;
            }
            return false;
        });
        m_menuItems->addItem(logMenu);
        m_debug = true;
        return true;
    }
    return false;
}
