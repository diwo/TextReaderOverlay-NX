#pragma once

#include <switch.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_CACHE_H
#include <stdexcept>
#include <functional>

#include <Log.hpp>

class FTLibrary {
public:
    FTLibrary() {
        if (FT_Init_FreeType(&m_library))
            throw std::runtime_error("FT_Init_FreeType failed");
    }
    ~FTLibrary() {
        FT_Done_FreeType(m_library);
    }
    operator FT_Library() const { return m_library; }

private:
    FT_Library m_library;
};

class FTCManager {
public:
    FTCManager(FTLibrary const &library, const char* fontPath) {
        if (FTC_Manager_New(library, 0, 0, 0, faceRequester, (void*)fontPath, &m_manager))
            throw std::runtime_error("FTC_Manager_New failed");
    }
    ~FTCManager() {
        FTC_Manager_Done(m_manager);
    }
    operator FTC_Manager() const { return m_manager; }

protected:
    static FT_Error faceRequester(FTC_FaceID faceId, FT_Library library, FT_Pointer reqData, FT_Face *face) {
        FT_Error err = FT_New_Face(library, (const char*)reqData, 0, face);
        if (err == FT_Err_Unknown_File_Format)
            Log::log("FT_New_Face failed: FT_Err_Unknown_File_Format");
        else if (err)
            Log::log("FT_New_Face failed: cannot open font file");
        return err;
    }

private:
    FTC_Manager m_manager;
};

class Font {
public:
    Font(const char* path) : m_cacheManager(m_library, path)
    {
        if (FTC_SBitCache_New(m_cacheManager, &m_sbitCache))
            throw std::runtime_error("FTC_SBitCache_New failed");
        if (FTC_CMapCache_New(m_cacheManager, &m_cmapCache))
            throw std::runtime_error("FTC_CMapCache_New failed");
    }

    void print(const char* str, s32 x, s32 y, u32 fontSize, std::function<void(s32 x, s32 y, u8 grad)> const &setPixel) const;

private:
    FTLibrary m_library;
    FTCManager m_cacheManager;
    FTC_SBitCache m_sbitCache;
    FTC_CMapCache m_cmapCache;
};
