#include <tesla.hpp>

#include <Overlay.hpp>
#include <MainMenu.hpp>

const char* TextReaderOverlay::VERSION = "v1.0";

tsl::Gui* TextReaderOverlay::onSetup() {
    return new MainMenu();
}

tsl::Overlay* overlayLoad() {
    return new TextReaderOverlay();
}
