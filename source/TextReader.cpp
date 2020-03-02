#include <TextReader.hpp>

#include <tesla.hpp>
#include <string.h>
#include <math.h>

#include <Config.hpp>
#include <Log.hpp>

std::string TextReaderChunk::EMPTY_STRING = "";

void TextReaderChunk::loadText(FILE *file) {
    if (m_lines != nullptr) {
        return;
    }
    else if (fseek(file, m_fileOffset, SEEK_SET)) {
        Log::log("Failed to load chunk");
    }
    else {
        m_lines = new std::vector<std::string>(MAX_SIZE, std::string());
        char buf[256];
        u32 line = 0;
        u32 eol_idx;
        while (line < MAX_SIZE && (fgets(buf, sizeof(buf), file) != nullptr)) {
            eol_idx = strcspn(buf, "\r\n");
            buf[eol_idx] = 0;
            m_lines->at(line).append(buf);
            if (eol_idx < sizeof(buf) - 1) {
                line++;
            }
        }
    }
}

void TextReaderChunk::unloadText() {
    if (m_lines != nullptr) {
        delete m_lines;
        m_lines = nullptr;
    }
}

std::string& TextReaderChunk::getLine(u32 lineOffset) const {
    if (m_lines != nullptr && lineOffset < m_lines->size())
        return m_lines->at(lineOffset);
    else
        return EMPTY_STRING;
}

TextReader::TextReader(std::string const &path)
    : m_path(path),
      m_totalLines(0),
      m_lineNum(0),
      m_chunkMid(0),
      m_loading(false),
      m_loaded(false),
      m_font("sdmc:/switch/.overlays/TextReaderOverlay/fonts/UbuntuMono/UbuntuMono-Regular.ttf"),
      m_size(10),
      m_panx(0),
      m_debug(false)
{
    auto j = Config::read();
    auto resume = j["files"][m_path].find("resume");
    if (resume != j["files"][m_path].end()) {
        m_lineNum = *resume;
    }
    auto bookmarks = j["files"][m_path].find("bookmarks");
    if (bookmarks != j["files"][m_path].end()) {
        for (auto &b : *bookmarks) {
            m_bookmarks.insert((u32)b);
        }
    }
}
TextReader::~TextReader() {
    if (m_file) fclose(m_file);
}

tsl::elm::Element* TextReader::createUI() {
    return new tsl::elm::CustomDrawer([this](tsl::gfx::Renderer *renderer, u16 x, u16 y, u16 w, u16 h) {
        renderer->fillScreen(a({ 0x0, 0x0, 0x0, 0xD }));

        if (!m_loading) {
            renderer->drawString("Loading... May take a few seconds", false, 20, 50, 16, a(0xFFFF));
            m_loading = true;
            return;
        }
        else if (!m_loaded) {
            m_file = fopen(m_path.c_str(), "r");
            if (m_file) {
                Log::log("Opened file %s", m_path.c_str());
                int c;
                u32 line = 0;
                m_chunks.push_back(TextReaderChunk(ftell(m_file)));
                while ((c = fgetc(m_file)) != EOF) {
                    if (c == '\n') {
                        ++line;
                        if (line % TextReaderChunk::MAX_SIZE == 0)
                            m_chunks.push_back(TextReaderChunk(ftell(m_file)));
                    }
                }
                m_totalLines = line + 1;
                scrollTo(m_lineNum);
            }
            else {
                Log::log("Could not open file %s", m_path.c_str());
            }
            m_loaded = true;
            return;
        }
        else if (!m_file) {
            renderer->drawString("Could not open file", false, 20, 50, 16, a(0xFFFF));
            return;
        }

        const size_t numLinesToShow = 100;
        for (u32 i = 0; i < numLinesToShow; ++i) {
            u32 chunk = (m_lineNum + i) / TextReaderChunk::MAX_SIZE;
            u32 line = (m_lineNum + i) % TextReaderChunk::MAX_SIZE;

            if (chunk < m_chunks.size()) {
                if (m_bookmarks.find(m_lineNum + i) != m_bookmarks.end()) {
                    renderer->drawRect(0, i * m_size, tsl::cfg::FramebufferWidth, 1, a({ 0x6, 0x1, 0x1, 0xF }));
                }
                printLn(
                    m_chunks[chunk].getLine(line),
                    10 + m_panx * m_size,
                    10 + i * m_size,
                    m_size,
                    renderer);
            }
        }

        u32 progressY = m_lineNum * (tsl::cfg::FramebufferHeight - 20) / m_totalLines;
        renderer->drawRect(0, progressY, 1, 20, a({ 0x8, 0x8, 0x8, 0xF }));

        if (m_debug)
            renderer->drawString(std::to_string(m_fps).c_str(), false, tsl::cfg::FramebufferWidth - 20, 10, 10, a(0xFFFF));
    });
}

void TextReader::printLn(std::string const &text, s32 x, s32 y, u32 fontSize, tsl::gfx::Renderer *renderer) const {
    m_font.print(text.c_str(), x, y, fontSize, [renderer](s32 x, s32 y, u8 grad) {
        renderer->setPixelBlendSrc(x, y, a({ 0xF, 0xF, 0xF, (u8)(grad >> 4) }));
    });
}

bool TextReader::handleInput(u64 keysDown, u64 keysHeld, touchPosition touchInput, JoystickPosition leftJoyStick, JoystickPosition rightJoyStick) {
    if (keysHeld & KEY_ZR) {
        if (keysHeld & KEY_LSTICK_UP)
            scrollTo(0);
        if (keysHeld & KEY_LSTICK_DOWN)
            scroll(m_totalLines);
        if (keysHeld & KEY_LSTICK_LEFT)
            scroll(-1000);
        if (keysHeld & KEY_LSTICK_RIGHT)
            scroll(1000);
    }
    else if (keysHeld & KEY_ZL) {
        if (keysHeld & KEY_LSTICK_UP)
            scroll(-20);
        if (keysHeld & KEY_LSTICK_DOWN)
            scroll(20);
        if (keysHeld & KEY_LSTICK_LEFT)
            scroll(-200);
        if (keysHeld & KEY_LSTICK_RIGHT)
            scroll(200);
    }
    else {
        if (keysHeld & KEY_LSTICK_UP)
            scroll(-2);
        if (keysHeld & KEY_LSTICK_DOWN)
            scroll(2);
        if (keysHeld & KEY_LSTICK_LEFT)
            scroll(-1);
        if (keysHeld & KEY_LSTICK_RIGHT)
            scroll(1);
    }

    if (keysHeld & KEY_RSTICK_UP)
        scroll(-1);
    if (keysHeld & KEY_RSTICK_DOWN)
        scroll(1);
    if (keysHeld & KEY_RSTICK_LEFT)
        m_panx++;
    if (keysHeld & KEY_RSTICK_RIGHT)
        m_panx--;
    if (keysDown & KEY_RSTICK)
        m_panx = 0;

    if (keysDown & KEY_DUP)
        m_size++;
    if (keysDown & KEY_DDOWN)
        m_size--;

    if (keysDown & KEY_X)
        tsl::Overlay::get()->hide();

    if (keysDown & KEY_Y)
        toggleBookmark();
    if (keysDown & KEY_L)
        previousBookmark();
    if (keysDown & KEY_R)
        nextBookmark();

    if (keysDown & KEY_B)
        close();

    if (keysDown & KEY_MINUS)
        m_debug = !m_debug;

    return true;
}

void TextReader::scrollTo(u32 line) {
    scroll(line - m_lineNum);
}

void TextReader::scroll(s32 offset) {
    u32 newLineNum = std::clamp((s32)m_lineNum + offset, 0, (s32)m_totalLines);
    u32 newChunk = newLineNum / TextReaderChunk::MAX_SIZE;
    u32 newOffset = newLineNum % TextReaderChunk::MAX_SIZE;

    // upwards
    if (newChunk < m_chunkMid &&
        (newChunk < m_chunkMid - 1 || newOffset < TextReaderChunk::MAX_SIZE / 2))
    {
        for (u32 chunk = m_chunkMid + 1; chunk > newChunk + 1; --chunk) {
            unloadText(chunk);
        }
        m_chunkMid = newChunk;
    }
    // downwards
    else if (newChunk > m_chunkMid &&
             (newChunk > m_chunkMid + 1 || newOffset > TextReaderChunk::MAX_SIZE / 2))
    {
        for (u32 chunk = std::max(0, (s32)m_chunkMid - 1); chunk < newChunk - 1; ++chunk) {
            unloadText(chunk);
        }
        m_chunkMid = newChunk;
    }

    loadText(m_chunkMid);
    if (m_chunkMid > 0)
        loadText(m_chunkMid - 1);
    if (m_chunkMid < m_chunks.size() - 1)
        loadText(m_chunkMid + 1);

    m_lineNum = newLineNum;
}

void TextReader::loadText(u32 chunk) {
    if (chunk < m_chunks.size()) {
        m_chunks[chunk].loadText(m_file);
    }
}

void TextReader::unloadText(u32 chunk) {
    if (chunk < m_chunks.size()) {
        m_chunks[chunk].unloadText();
    }
}

void TextReader::toggleBookmark() {
    if (m_bookmarks.find(m_lineNum) == m_bookmarks.end())
        m_bookmarks.insert(m_lineNum);
    else
        m_bookmarks.erase(m_lineNum);

    Config::update([this](json &j) {
        j["files"][m_path]["bookmarks"] = m_bookmarks;
    });
}

void TextReader::previousBookmark() {
    u32 prev;
    bool any = false;
    for (auto b : m_bookmarks) {
        if (b >= m_lineNum) {
            if (any)
                scrollTo(prev);
            break;
        }
        prev = b;
        any = true;
    }
    if (any)
        scrollTo(prev);
}

void TextReader::nextBookmark() {
    for (auto b : m_bookmarks) {
        if (b > m_lineNum) {
            scrollTo(b);
            break;
        }
    }
}

void TextReader::close() const {
    Config::update([this](json &j) {
        j["files"][m_path]["resume"] = m_lineNum;
    });
    tsl::goBack();
}
