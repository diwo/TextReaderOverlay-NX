#pragma once

#include <tesla.hpp>
#include <list>
#include <chrono>

#include <Font.hpp>

class TextReader : public tsl::Gui {
public:
    TextReader(const char* path);
    ~TextReader();

    tsl::Element* createUI() override;
    void handleInputs(s64 keysDown, s64 keysHeld, JoystickPosition joyStickPosLeft, JoystickPosition joyStickPosRight, u32 touchX, u32 touchY) override;

    void preDraw(tsl::Screen *screen) override {
        screen->fillScreen(tsl::a({ 0x0, 0x0, 0x0, 0xD }));
    }

    void postDraw(tsl::Screen *screen) override {
        auto newTimer = std::chrono::steady_clock::now();
        m_fps = 1000 / std::chrono::duration_cast<std::chrono::milliseconds>(newTimer - m_timer).count();
        m_timer = newTimer;
    }

protected:
    inline void printLn(const char* text, u32 x, u32 y, u32 fontSize, tsl::Screen *screen);

private:
    FILE *m_file;
    Font m_font;
    std::list<std::string> m_text;
    int m_size;
    int m_scroll_x;
    int m_scroll_y;

    std::chrono::steady_clock::time_point m_timer = std::chrono::steady_clock::now();
    long m_fps;
    bool m_debug;
};
