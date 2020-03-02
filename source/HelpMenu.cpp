#include <HelpMenu.hpp>

#include <Overlay.hpp>

tsl::elm::Element* HelpMenu::createUI() {
    auto *frame = new tsl::elm::OverlayFrame("", "");

    frame->setContent(new tsl::elm::CustomDrawer([](tsl::gfx::Renderer *renderer, u16 x, u16 y, u16 w, u16 h) {
        u32 offset, i;

        offset = 50, i = 0;
        renderer->drawString("File Browser Controls", false, 20, offset, 30, a(0xFFFF));
        showKeybind(renderer, offset, i++, "\uE0A3", "Toggle Favorite");

        offset = 180, i = 0;
        renderer->drawString("Reader Controls", false, 20, offset, 30, a(0xFFFF));
        showKeybind(renderer, offset, i++, "\uE081", "Scroll");
        showKeybind(renderer, offset, i++, "\uE085 \uE081", "Scroll Faster");
        showKeybind(renderer, offset, i++, "\uE086 \uE081 \uE091 \uE090", "Scroll Even Faster");
        showKeybind(renderer, offset, i++, "\uE086 \uE081 \uE092 \uE093", "Scroll to Top / Bottom");
        showKeybind(renderer, offset, i++, "\uE07D", "Adjust Font Size");
        showKeybind(renderer, offset, i++, "\uE082", "Scroll Sideways");
        showKeybind(renderer, offset, i++, "\uE08B", "Beginning of Line");
        showKeybind(renderer, offset, i++, "\uE0A3", "Toggle Bookmark");
        showKeybind(renderer, offset, i++, "\uE0A4 \uE0A4", "Prev/Next Bookmark");
        showKeybind(renderer, offset, i++, "\uE0A2", "Hide Overlay");
    }));

    return frame;
}

inline void HelpMenu::showKeybind(tsl::gfx::Renderer *renderer, u32 offset, u32 i, const char* keys, const char* desc) {
    renderer->drawString(keys, false, 20, offset + 50 + 32 * i, 24, a(0xFFFF));
    renderer->drawString(desc, false, 160, offset + 50 + 32 * i, 24, a(0xFFFF));
}
