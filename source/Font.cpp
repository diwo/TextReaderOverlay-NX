#include <Font.hpp>

FTLibrary::FTLibrary() {
    if (FT_Init_FreeType(&m_library))
        throw std::runtime_error("FT_Init_FreeType failed");
}

FTLibrary::~FTLibrary() {
    FT_Done_FreeType(m_library);
}

FTCManager::FTCManager(FTLibrary &library, const char* fontPath) {
    if (FTC_Manager_New(library, 0, 0, 0, faceRequester, (void*)fontPath, &m_manager))
        throw std::runtime_error("FTC_Manager_New failed");
}

FTCManager::~FTCManager() {
    FTC_Manager_Done(m_manager);
}

FT_Error FTCManager::faceRequester(FTC_FaceID faceId, FT_Library library, FT_Pointer reqData, FT_Face *face) {
    FT_Error err = FT_New_Face(library, (const char*)reqData, 0, face);
    if (err == FT_Err_Unknown_File_Format)
        Log::log("FT_New_Face failed: FT_Err_Unknown_File_Format");
    else if (err)
        Log::log("FT_New_Face failed: cannot open font file");
    return err;
}

Font::Font(const char* path)
: m_cacheManager(m_library, path)
{
    if (FTC_SBitCache_New(m_cacheManager, &m_sbitCache))
        throw std::runtime_error("FTC_SBitCache_New failed");
    if (FTC_CMapCache_New(m_cacheManager, &m_cmapCache))
        throw std::runtime_error("FTC_CMapCache_New failed");
}
Font::~Font() {}

void Font::print(const char* str, u32 x, u32 y, u32 fontSize, std::function<void(u32 x, u32 y, u8 grad)> setPixel) {
    u32 pen_x = x;
    u32 pen_y = y;
    uint32_t chr;
    ssize_t units;
    size_t len = strlen(str);
    FT_UInt glyphIndex;
    FTC_SBit sbit;
    FTC_ImageTypeRec imageType;

    imageType.face_id = 0;
    imageType.width = fontSize;
    imageType.height = fontSize;
    imageType.flags = FT_LOAD_DEFAULT;

    for (size_t i = 0; i < len; ) {
        units = decode_utf8(&chr, (const uint8_t*)&str[i]);
        if (units <= 0) break;
        i += units;

        if (!(glyphIndex = FTC_CMapCache_Lookup(m_cmapCache, imageType.face_id, 0, chr)))
            continue;

        if (FTC_SBitCache_Lookup(m_sbitCache, &imageType, glyphIndex, &sbit, 0))
            break;

        for (u32 j = 0; j < sbit->height; j++) {
            for (u32 i = 0; i < sbit->width; i++) {
                u8 grad = sbit->buffer[sbit->pitch * j + i];
                setPixel(
                    pen_x + sbit->left + i,
                    pen_y - sbit->top + j,
                    grad);
            }
        }

        pen_x += sbit->xadvance;
        pen_y += sbit->yadvance;
    }
}
