#include <LogMenu.hpp>

#include <Log.hpp>

tsl::elm::Element* LogMenu::createUI() {
    auto *frame = new tsl::elm::OverlayFrame("Log Menu", "");

    frame->setContent(new tsl::elm::CustomDrawer([](tsl::gfx::Renderer *renderer, u16 x, u16 y, u16 w, u16 h) {
        Log::forEach([renderer](std::string const &msg, int i) {
            renderer->drawString(msg.c_str(), false, 20, 80 + i*12, 12, a(0xFFFF));
        });
    }));

    return frame;
}
