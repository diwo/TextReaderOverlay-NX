#include <tesla.hpp>

#include <HelpMenu.hpp>
#include <Overlay.hpp>

tsl::Element* HelpMenu::createUI() {
    auto *rootFrame = new tsl::element::Frame();

    rootFrame->addElement(new tsl::element::CustomDrawer(0, 0, FB_WIDTH, FB_HEIGHT, [](u16 x, u16 y, tsl::Screen *screen) {
        auto show = [screen](u32 offset, u32 i, const char* keys, const char* desc) {
            screen->drawString(keys, false, 20, offset + 50 + 32 * i, 24, tsl::a(0xFFFF));
            screen->drawString(desc, false, 160, offset + 50 + 32 * i, 24, tsl::a(0xFFFF));
        };

        u32 offset, i;

        offset = 50, i = 0;
        screen->drawString("File Browser Controls", false, 20, offset, 30, tsl::a(0xFFFF));
        show(offset, i++, "\uE0A3", "Toggle Favorite");

        offset = 180, i = 0;
        screen->drawString("Reader Controls", false, 20, offset, 30, tsl::a(0xFFFF));
        show(offset, i++, "\uE081", "Scroll");
        show(offset, i++, "\uE085 \uE081", "Scroll Faster");
        show(offset, i++, "\uE086 \uE081 \uE091 \uE090", "Scroll Even Faster");
        show(offset, i++, "\uE086 \uE081 \uE092 \uE093", "Scroll to Top / Bottom");
        show(offset, i++, "\uE07D", "Adjust Font Size");
        show(offset, i++, "\uE082", "Scroll Sideways");
        show(offset, i++, "\uE08B", "Beginning of Line");
        show(offset, i++, "\uE0A3", "Toggle Bookmark");
        show(offset, i++, "\uE0A4 \uE0A4", "Prev/Next Bookmark");
        show(offset, i++, "\uE0A2", "Hide Overlay");
        show(offset, i++, "\uE085 \uE086 \uE0EF", "Toggle Overlay");

        screen->drawString(TextReaderOverlay::VERSION, true, 420, 710, 10, tsl::a(0xFFFF));
    }));

    return rootFrame;
}
