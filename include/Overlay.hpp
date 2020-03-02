#pragma once

#include <tesla.hpp>

#include <MainMenu.hpp>

#ifndef TEXT_READER_VERSION
#define TEXT_READER_VERSION "v1.1"
#endif

class TextReaderOverlay : public tsl::Overlay {
public:
    std::unique_ptr<tsl::Gui> loadInitialGui() override {
        return initially<MainMenu>();
    }
};
