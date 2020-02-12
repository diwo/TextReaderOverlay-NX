#include <tesla.hpp>

#include <MainMenu.hpp>
#include <FileSelect.hpp>
#include <LogMenu.hpp>

MainMenu::MainMenu() {}
MainMenu::~MainMenu() {}

tsl::Element* MainMenu::createUI() {
    this->setTitle("Text Reader");

    auto rootFrame = new tsl::element::Frame();
    auto menuItems = new tsl::element::List();

    auto fileSelect = new tsl::element::ListItem("Select File...");
    fileSelect->setClickListener([](s64 keys) {
        if (keys & KEY_A) {
            tsl::Gui::changeTo(new FileSelect("sdmc:/"));
            return true;
        }
        return false;
    });

    auto logMenu = new tsl::element::ListItem("Show Logs");
    logMenu->setClickListener([](s64 keys) {
        if (keys & KEY_A) {
            tsl::Gui::changeTo(new LogMenu());
            return true;
        }
        return false;
    });

    rootFrame->addElement(menuItems);
    menuItems->addItem(fileSelect);
    menuItems->addItem(logMenu);

    return rootFrame;
}
