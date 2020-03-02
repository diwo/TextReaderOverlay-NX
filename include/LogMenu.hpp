#pragma once

#include <tesla.hpp>

class LogMenu : public tsl::Gui {
public:
    tsl::elm::Element* createUI() override;
};
