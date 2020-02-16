#include <tesla.hpp>

class TextReaderOverlay : public tsl::Overlay {
public:
    static const char* VERSION;

    tsl::Gui* onSetup() override;
};
