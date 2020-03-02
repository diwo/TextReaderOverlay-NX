#pragma once

#include <tesla.hpp>

class HelpMenu : public tsl::Gui {
public:
    tsl::elm::Element* createUI() override;

private:
    static inline void showKeybind(tsl::gfx::Renderer *renderer, u32 offset, u32 i, const char* keys, const char* desc);
};
