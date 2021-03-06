#pragma once

#include "defs.h"
#include "math.h"

// These constants are EGA-Hardware limits depending on render mode
// They're mostly here for reference but most things won't be hardcoded with it
#define EGA_RES_WIDTH 320
#define EGA_RES_HEIGHT 200
#define EGA_TEXT_CHAR_WIDTH 8
#define EGA_TEXT_CHAR_HEIGHT 8
#define EGA_PIXEL_HEIGHT 1.2f
#define EGA_PIXEL_WIDTH 1.00f

#define EGA_COLORS 64
#define EGA_COLOR_UNDEFINED (EGA_COLORS) // used for palette generation, marks a color entry as able to be anything
#define EGA_COLOR_UNUSED (EGA_COLORS + 1) //usedf for palette generation, marks a color entry as not usable
#define EGA_PALETTE_COLORS 16
#define EGA_TEXT_RES_WIDTH (EGA_RES_WIDTH / EGA_TEXT_CHAR_WIDTH)
#define EGA_TEXT_RES_HEIGHT (EGA_RES_HEIGHT / EGA_TEXT_CHAR_HEIGHT)
#define EGA_PIXELS (EGA_RES_WIDTH * EGA_RES_HEIGHT)
#define EGA_BYTES (EGA_PIXELS / 8)
#define EGA_PLANES 4

// byte value for alpha
#define EGA_ALPHA 0xFF

// EGA Supports one of 64 colors
typedef byte EGAColor;
typedef byte EGAPColor; //(0-15)
typedef struct {
   EGAColor colors[EGA_PALETTE_COLORS];
} EGAPalette;

void egaStartup();

// Takes an EGA color index (0-63) and returns the 3-byte RGB
// useful externally for reference graphics
extern ColorRGB g_egaToRGBTable[64];
#define egaGetColor(c) g_egaToRGBTable[c]

// EGATextures are encoded images consistenting of 4 bits per pixel, referring to a palette index
// These were stored in 4 seperate bit planes but are intervleaved on the backend here
// The texture handles all transparency, byte offsets, and rendering
typedef struct EGATexture EGATexture;

EGATexture *egaTextureCreate(u32 width, u32 height);
EGATexture *egaTextureCreateCopy(EGATexture const *other);
void egaTextureDestroy(EGATexture *self);

// encoding and decoding from an rgb texture
typedef struct Texture Texture;
EGATexture *egaTextureCreateFromTextureEncode(Texture *source, EGAPalette *targetPalette, EGAPalette *resultPalette);

// target must exist and must match ega's size, returns !0 on success
int egaTextureDecode(EGATexture *self, Texture* target, EGAPalette *palette);

// binary serialization
int egaTextureSerialize(EGATexture *self, byte **outBuff, u64 *size);
EGATexture *egaTextureDeserialize(byte *buff, u64 size);

Int2 egaTextureGetSize(EGATexture const *self);

void egaTextureResize(EGATexture *self, u32 width, u32 height);

// EGARegions are passed to all draw calls
// They define an origin and clipping rect for the draw
typedef Recti EGARegion;

// useful in certian circumstances, dont use this for normal calls
// passing NULL to a render call will use this for the target automatically
EGARegion *egaTextureGetFullRegion(EGATexture *self);

EGAPColor egaTextureGetColorAt(EGATexture *self, u32 x, u32 y, EGARegion *vp = nullptr);


// The font factory manages fonts, theres only one "font" in EGA
// Font in this case means color, background/foreground
typedef struct EGAFontFactory EGAFontFactory;
typedef struct EGAFont EGAFont;

/*
Image must be:
- 256x112 with 256 8x14 characters organized according to ascii
- solid 1 alpha (no transparency)
- 2-color palette; 0 or background and 1 for foreground
*/
EGAFontFactory *egaFontFactoryCreate(EGATexture *font);
void egaFontFactoryDestroy(EGAFontFactory *self);
EGAFont *egaFontFactoryGetFont(EGAFontFactory *self, EGAColor bgColor, EGAColor fgColor);


void egaClear(EGATexture *target, EGAPColor color, EGARegion *vp = nullptr);
void egaClearAlpha(EGATexture *target);
void egaColorReplace(EGATexture *target, EGAPColor oldCOlor, EGAPColor newColor);
void egaRenderTexture(EGATexture *target, Int2 pos, EGATexture *tex, EGARegion *vp = nullptr);
void egaRenderTexturePartial(EGATexture *target, Int2 pos, EGATexture *tex, Recti uv, EGARegion *vp = nullptr);
void egaRenderPoint(EGATexture *target, Int2 pos, EGAPColor color, EGARegion *vp = nullptr);
void egaRenderLine(EGATexture *target, Int2 pos1, Int2 pos2, EGAPColor color, EGARegion *vp = nullptr);
void egaRenderLineRect(EGATexture *target, Recti r, EGAPColor color, EGARegion *vp = nullptr);
void egaRenderRect(EGATexture *target, Recti r, EGAPColor color, EGARegion *vp = nullptr);

void egaRenderCircle(EGATexture *target, Int2 pos, int radius, EGAPColor color, EGARegion *vp = nullptr);
void egaRenderEllipse(EGATexture *target, Recti r, EGAPColor color, EGARegion *vp = nullptr);
void egaRenderEllipseQB(EGATexture *target, Int2 pos, int radius, double aspect, EGAPColor color, EGARegion *vp = nullptr);

void egaRenderTextSingleChar(EGATexture *target, const char c, Int2 pos, EGAFont *font, int spaces);
void egaRenderText(EGATexture *target, const char *text, Int2 pos, EGAFont *font);
void egaRenderTextWithoutSpaces(EGATexture *target, const char *text, Int2 pos, EGAFont *font);
