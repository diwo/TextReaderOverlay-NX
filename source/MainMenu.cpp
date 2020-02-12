#include <tesla.hpp>

#include <MainMenu.hpp>
#include <TextReader.hpp>
#include <LogMenu.hpp>

MainMenu::MainMenu() {}
MainMenu::~MainMenu() {}

tsl::Element* MainMenu::createUI() {
    this->setTitle("Text Reader");

    auto rootFrame = new tsl::element::Frame();
    auto menuItems = new tsl::element::List();

    auto textReader = new tsl::element::ListItem("Open Reader");
    textReader->setClickListener([](s64 keys) {
        if (keys & KEY_A) {
            tsl::Gui::changeTo(new TextReader("sdmc:/switch/.overlays/TextReaderOverlay/sample.txt"));
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
    menuItems->addItem(textReader);
    menuItems->addItem(logMenu);

    return rootFrame;
}
