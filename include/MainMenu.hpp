#pragma once

#include <tesla.hpp>

class MainMenu : public tsl::Gui {
public:
    tsl::Element* createUI() override;
    void handleInputs(s64 keysDown, s64 keysHeld, JoystickPosition joyStickPosLeft, JoystickPosition joyStickPosRight, u32 touchX, u32 touchY) override;

private:
    tsl::element::List* m_menuItems;
    bool m_debug;
};
