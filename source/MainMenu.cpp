#include <tesla.hpp>

#include <MainMenu.hpp>
#include <FileSelect.hpp>
#include <FavoritesMenu.hpp>
#include <LogMenu.hpp>

tsl::Element* MainMenu::createUI() {
    this->setTitle("Text Reader");

    auto frame = new tsl::element::Frame();
    m_menuItems = new tsl::element::List();
    m_debug = false;

    auto fileSelect = new tsl::element::ListItem("Browse...");
    fileSelect->setClickListener([](s64 keys) {
        if (keys & KEY_A) {
            tsl::Gui::changeTo(new FileSelect("sdmc:/"));
            return true;
        }
        return false;
    });
    
    auto favorites = new tsl::element::ListItem("Favorites");
    favorites->setClickListener([](s64 keys) {
        if (keys & KEY_A) {
            tsl::Gui::changeTo(new FavoritesMenu());
            return true;
        }
        return false;
    });

    frame->addElement(m_menuItems);
    m_menuItems->addItem(fileSelect);
    m_menuItems->addItem(favorites);

    return frame;
}

void MainMenu::handleInputs(s64 keysDown, s64 keysHeld, JoystickPosition joyStickPosLeft, JoystickPosition joyStickPosRight, u32 touchX, u32 touchY) {
    if (!m_debug && (keysDown & KEY_MINUS)) {
        auto logMenu = new tsl::element::ListItem("Show Logs");
        logMenu->setClickListener([](s64 keys) {
            if (keys & KEY_A) {
                tsl::Gui::changeTo(new LogMenu());
                return true;
            }
            return false;
        });
        m_menuItems->addItem(logMenu);
        m_menuItems->layout();
        m_debug = true;
    }
}
