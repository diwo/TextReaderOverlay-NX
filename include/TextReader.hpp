#pragma once

#include <tesla.hpp>
#include <string>
#include <vector>
#include <chrono>

#include <Font.hpp>

class TextReaderChunk {
public:
    static const size_t MAX_SIZE = 1000;

    TextReaderChunk(long int fileOffset);
    ~TextReaderChunk();

    void loadText(FILE *file);
    void unloadText();
    std::string getLine(u32 lineOffset);

private:
    long int m_fileOffset;
    std::vector<std::string> *m_lines;
};

class TextReader : public tsl::Gui {
public:
    TextReader(std::string path);
    ~TextReader();

    tsl::Element* createUI() override;
    void handleInputs(s64 keysDown, s64 keysHeld, JoystickPosition joyStickPosLeft, JoystickPosition joyStickPosRight, u32 touchX, u32 touchY) override;

    void scrollTo(u32 line);
    void scroll(s32 offset);

    void preDraw(tsl::Screen *screen) override {
        screen->fillScreen(tsl::a({ 0x0, 0x0, 0x0, 0xD }));
    }

    void postDraw(tsl::Screen *screen) override {
        auto newTimer = std::chrono::steady_clock::now();
        m_fps = 1000 / std::chrono::duration_cast<std::chrono::milliseconds>(newTimer - m_timer).count();
        m_timer = newTimer;
    }

protected:
    inline void printLn(std::string text, u32 x, u32 y, u32 fontSize, tsl::Screen *screen);

private:
    void loadText(u32 chunk);
    void unloadText(u32 chunk);

private:
    std::string m_path;
    FILE *m_file;
    u32 m_lineNum;
    u32 m_chunkMid;
    std::vector<TextReaderChunk> m_chunks;

    u32 m_loading;
    bool m_loaded;

    Font m_font;
    u32 m_size;
    s32 m_panx;

    std::chrono::steady_clock::time_point m_timer = std::chrono::steady_clock::now();
    u32 m_fps;
    bool m_debug;
};
