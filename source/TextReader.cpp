#include <tesla.hpp>
#include <string.h>

#include <TextReader.hpp>
#include <Log.hpp>

TextReader::TextReader(const char* path)
: m_font("sdmc:/switch/.overlays/TextReaderOverlay/fonts/UbuntuMono/UbuntuMono-Regular.ttf"),
  m_text({}),
  m_size(10),
  m_scroll_x(0),
  m_scroll_y(0),
  m_debug(false)
{
    m_file = fopen(path, "r");
    if (m_file) {
        char buf[200];
        u8 line = 0;
        while (line < 50 && fgets(buf, sizeof(buf), m_file) != nullptr) {
            buf[strcspn(buf, "\r\n")] = 0;
            m_text.push_back(std::string(buf));
            line++;
        }
    }
}
TextReader::~TextReader() {
    if (m_file) fclose(m_file);
}

tsl::Element* TextReader::createUI() {
    tsl::element::Frame *rootFrame = new tsl::element::Frame();

    rootFrame->addElement(new tsl::element::CustomDrawer(0, 0, FB_WIDTH, FB_HEIGHT, [this](u16 x, u16 y, tsl::Screen *screen) {
        if (!m_file) {
            screen->drawString("Could not open file", true, 10, 10, 10, tsl::a(0xFFFF));
            return;
        }

        if (m_debug)
            screen->drawString(std::to_string(m_fps).c_str(), true, FB_WIDTH - 20, 10, 10, tsl::a(0xFFFF));

        int line = 0;
        for (std::string s : m_text) {
            printLn(
                s.c_str(),
                10 + m_scroll_x * m_size,
                10 + (line + m_scroll_y) * m_size,
                m_size,
                screen);
            line++;
        }
    }));

    return rootFrame;
}

void TextReader::printLn(const char* text, u32 x, u32 y, u32 fontSize, tsl::Screen *screen) {
    m_font.print(text, x, y, fontSize, [screen](u32 x, u32 y, u8 grad) {
        screen->setPixelBlendSrc(x, y, tsl::a({ 0xF, 0xF, 0xF, (u8)(grad >> 4) }));
    });
}

void TextReader::handleInputs(s64 keysDown, s64 keysHeld, JoystickPosition joyStickPosLeft, JoystickPosition joyStickPosRight, u32 touchX, u32 touchY) {
    if (keysHeld & (KEY_LSTICK_UP | KEY_RSTICK_UP))
        m_scroll_y--;
    if (keysHeld & (KEY_LSTICK_DOWN | KEY_RSTICK_DOWN))
        m_scroll_y++;

    if (keysHeld & KEY_RSTICK_LEFT)
        m_scroll_x--;
    if (keysHeld & KEY_RSTICK_RIGHT)
        m_scroll_x++;
    if (keysDown & KEY_RSTICK)
        m_scroll_x = 0;

    if (keysDown & KEY_DUP)
        m_size++;
    if (keysDown & KEY_DDOWN)
        m_size--;

    if (keysDown & KEY_X)
        tsl::Gui::playOutroAnimation();

    if (keysDown & KEY_MINUS)
        m_debug = !m_debug;
}
