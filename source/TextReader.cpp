#include <tesla.hpp>
#include <string.h>
#include <math.h>

#include <TextReader.hpp>
#include <Log.hpp>


TextReaderChunk::TextReaderChunk(long int fileOffset)
: m_fileOffset(fileOffset),
  m_lines(nullptr)
{}
TextReaderChunk::~TextReaderChunk() {
    unloadText();
}

void TextReaderChunk::loadText(FILE *file) {
    if (m_lines != nullptr) {
        return;
    } else if (fseek(file, m_fileOffset, SEEK_SET)) {
        Log::log("Failed to load chunk");
    } else {
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

std::string TextReaderChunk::getLine(u32 lineOffset) {
    if (m_lines != nullptr && lineOffset < m_lines->size())
        return m_lines->at(lineOffset);
    else
        return std::string();
}

TextReader::TextReader(std::string path)
: m_path(path),
  m_lineNum(0),
  m_chunkMid(0),
  m_loading(0),
  m_loaded(false),
  m_font("sdmc:/switch/.overlays/TextReaderOverlay/fonts/UbuntuMono/UbuntuMono-Regular.ttf"),
  m_size(10),
  m_panx(0),
  m_debug(false)
{}
TextReader::~TextReader() {
    if (m_file) fclose(m_file);
}

tsl::Element* TextReader::createUI() {
    tsl::element::Frame *rootFrame = new tsl::element::Frame();

    rootFrame->addElement(new tsl::element::CustomDrawer(0, 0, FB_WIDTH, FB_HEIGHT, [this](u16 x, u16 y, tsl::Screen *screen) {
        if (m_loading < 2) {
            screen->drawString("Loading... May take a few seconds", true, 20, 50, 16, tsl::a(0xFFFF));
            m_loading++;
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
                        line++;
                        if (line % TextReaderChunk::MAX_SIZE == 0)
                            m_chunks.push_back(TextReaderChunk(ftell(m_file)));
                    }
                }
                scrollTo(m_lineNum);
            }
            else {
                Log::log("Could not open file %s", m_path.c_str());
            }
            m_loaded = true;
            return;
        }
        else if (!m_file) {
            screen->drawString("Could not open file", true, 20, 50, 16, tsl::a(0xFFFF));
            return;
        }

        const size_t numLinesToShow = 100;
        for (u32 i = 0; i < numLinesToShow; i++) {
            u32 chunk = (m_lineNum + i) / TextReaderChunk::MAX_SIZE;
            u32 line = (m_lineNum + i) % TextReaderChunk::MAX_SIZE;

            if (chunk < m_chunks.size()) {
                printLn(
                    m_chunks[chunk].getLine(line).c_str(),
                    10 + m_panx * m_size,
                    10 + i * m_size,
                    m_size,
                    screen);
            }
        }

        if (m_debug)
            screen->drawString(std::to_string(m_fps).c_str(), true, FB_WIDTH - 20, 10, 10, tsl::a(0xFFFF));
    }));

    return rootFrame;
}

void TextReader::printLn(std::string text, u32 x, u32 y, u32 fontSize, tsl::Screen *screen) {
    m_font.print(text.c_str(), x, y, fontSize, [screen](u32 x, u32 y, u8 grad) {
        if (x < FB_WIDTH)
            screen->setPixelBlendSrc(x, y, tsl::a({ 0xF, 0xF, 0xF, (u8)(grad >> 4) }));
    });
}

void TextReader::handleInputs(s64 keysDown, s64 keysHeld, JoystickPosition joyStickPosLeft, JoystickPosition joyStickPosRight, u32 touchX, u32 touchY) {
    if (keysHeld & KEY_LSTICK_UP)
        scroll(-2);
    if (keysHeld & KEY_LSTICK_DOWN)
        scroll(2);
    if (keysHeld & KEY_LSTICK_LEFT)
        scroll(-20);
    if (keysHeld & KEY_LSTICK_RIGHT)
        scroll(20);

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
        tsl::Gui::playOutroAnimation();

    if (keysDown & KEY_MINUS)
        m_debug = !m_debug;
}

void TextReader::scrollTo(u32 line) {
    scroll(line - m_lineNum);
}

void TextReader::scroll(s32 offset) {
    u32 newLineNum = std::max((s32)m_lineNum + offset, 0);
    u32 newChunk = newLineNum / TextReaderChunk::MAX_SIZE;
    u32 newOffset = newLineNum % TextReaderChunk::MAX_SIZE;

    // upwards
    if (newChunk < m_chunkMid &&
        (newChunk < m_chunkMid - 1 || newOffset < TextReaderChunk::MAX_SIZE / 2))
    {
        for (u32 chunk = m_chunkMid + 1; chunk > newChunk + 1; chunk--) {
            unloadText(chunk);
        }
        m_chunkMid = newChunk;
    }
    // downwards
    else if (newChunk > m_chunkMid &&
             (newChunk > m_chunkMid + 1 || newOffset > TextReaderChunk::MAX_SIZE / 2))
    {
        for (u32 chunk = std::max((s32)m_chunkMid - 1, 0); chunk < newChunk - 1; chunk++) {
            unloadText(chunk);
        }
        m_chunkMid = newChunk;
    }

    loadText(m_chunkMid);
    if (m_chunkMid > 0)
        loadText(m_chunkMid-1);
    if (m_chunkMid < m_chunks.size() - 1)
        loadText(m_chunkMid+1);

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
