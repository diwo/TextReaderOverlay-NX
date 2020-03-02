#include <Font.hpp>

void Font::print(const char* str, s32 x, s32 y, u32 fontSize, std::function<void(s32 x, s32 y, u8 grad)> const &setPixel) const {
    s32 pen_x = x;
    s32 pen_y = y;
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

    size_t i = 0;
    while (i < len) {
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
