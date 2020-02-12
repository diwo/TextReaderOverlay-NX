#pragma once

#include <switch.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_CACHE_H
#include <functional>
#include <stdexcept>

#include <Log.hpp>

class FTLibrary {
public:
    FTLibrary();
    ~FTLibrary();
    operator FT_Library() const { return m_library; }

private:
    FT_Library m_library;
};

class FTCManager {
public:
    FTCManager(FTLibrary &library, const char* fontPath);
    ~FTCManager();
    operator FTC_Manager() const { return m_manager; }

protected:
    static FT_Error faceRequester(FTC_FaceID faceId, FT_Library library, FT_Pointer reqData, FT_Face *face);

private:
    FTC_Manager m_manager;
};

class Font {
public:
    Font(const char* path);
    ~Font();

    void print(const char* str, u32 x, u32 y, u32 fontSize, std::function<void(u32 x, u32 y, u8 grad)> setPixel);

private:
    FTLibrary m_library;
    FTCManager m_cacheManager;
    FTC_SBitCache m_sbitCache;
    FTC_CMapCache m_cmapCache;
};
