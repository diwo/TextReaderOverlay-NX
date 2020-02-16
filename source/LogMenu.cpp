#include <tesla.hpp>

#include <LogMenu.hpp>
#include <Log.hpp>

tsl::Element* LogMenu::createUI() {
    auto *rootFrame = new tsl::element::Frame();

    rootFrame->addElement(new tsl::element::CustomDrawer(0, 0, FB_WIDTH, FB_HEIGHT, [](u16 x, u16 y, tsl::Screen *screen) {
        Log::forEach([screen](const char* msg, int i) {
            screen->drawString(msg, false, 20, 80 + i*12, 12, tsl::a(0xFFFF));
        });
    }));

    return rootFrame;
}

void LogMenu::preDraw(tsl::Screen *screen) {
    screen->fillScreen(tsl::a({ 0x0, 0x0, 0x0, 0xD }));
    screen->drawString("Log Menu", false, 20, 50, 30, tsl::a(0xFFFF));
}
