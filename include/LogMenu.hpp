#pragma once

#include <tesla.hpp>

class LogMenu : public tsl::Gui {
public:
    tsl::Element* createUI();
    void preDraw(tsl::Screen *screen) override;
};
