#pragma once

#include <tesla.hpp>
#include <string>
#include <vector>
#include <set>
#include <functional>
#include <chrono>

#include <Font.hpp>

class TextReaderChunk {
public:
    static const size_t MAX_SIZE = 1000;

    TextReaderChunk(long int fileOffset)
        : m_fileOffset(fileOffset),
          m_lines(nullptr)
    {}
    ~TextReaderChunk() { unloadText(); }

    void loadText(FILE *file);
    void unloadText();
    std::string& getLine(u32 lineOffset) const;

private:
    static std::string EMPTY_STRING;
    long int m_fileOffset;
    std::vector<std::string> *m_lines;
};

class TextReader : public tsl::Gui {
public:
    TextReader(std::string const &path);
    ~TextReader();

    tsl::elm::Element* createUI() override;
    bool handleInput(u64 keysDown, u64 keysHeld, touchPosition touchInput, JoystickPosition leftJoyStick, JoystickPosition rightJoyStick) override;

    void scrollTo(u32 line);
    void scroll(s32 offset);
    void toggleBookmark();
    void previousBookmark();
    void nextBookmark();
    void close() const;

    void update() override {
        auto now = std::chrono::steady_clock::now();
        m_timeAggregate += std::chrono::duration_cast<std::chrono::milliseconds>(now - m_timePrev);
        ++m_timeTicks;
        m_timePrev = now;
        if (m_timeAggregate > 200ms) {
            m_fps = 1000 * m_timeTicks / m_timeAggregate.count();
            m_timeAggregate = 0ms;
            m_timeTicks = 0;
        }
    }

protected:
    inline void printLn(std::string const &text, s32 x, s32 y, u32 fontSize, tsl::gfx::Renderer *renderer) const;

private:
    void loadText(u32 chunk);
    void unloadText(u32 chunk);

private:
    std::string m_path;
    FILE *m_file;
    u32 m_totalLines;
    u32 m_lineNum;
    u32 m_chunkMid;
    std::vector<TextReaderChunk> m_chunks;

    bool m_loading;
    bool m_loaded;

    Font m_font;
    u32 m_size;
    s32 m_panx;
    std::set<u32> m_bookmarks;

    std::chrono::steady_clock::time_point m_timePrev;
    std::chrono::milliseconds m_timeAggregate;
    u32 m_timeTicks;
    u32 m_fps;
    bool m_debug;
};
