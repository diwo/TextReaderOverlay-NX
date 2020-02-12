#pragma once

#include <tesla.hpp>

class MainMenu : public tsl::Gui {
public:
    MainMenu();
    ~MainMenu();

    tsl::Element* createUI() override;
};
