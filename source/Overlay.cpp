#include <tesla.hpp>

#include <MainMenu.hpp>

class TextReaderOverlay : public tsl::Overlay {
public:
    TextReaderOverlay() {}
    ~TextReaderOverlay() {}

    tsl::Gui* onSetup() {
        return new MainMenu();
    }
};

tsl::Overlay* overlayLoad() {
    return new TextReaderOverlay();
}
