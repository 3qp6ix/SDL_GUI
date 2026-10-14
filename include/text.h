#ifndef TEXT_H
#define TEXT_H

#include <stdint.h> // uint8_t
#include "SDL2/SDL.h" // SDL_Surface,SDL_Rect
#include <stdbool.h> // bool

#define FONT_DATA_CAPACITY 128

typedef struct {
    uint8_t width;
    uint8_t height;
    uint8_t increment;
    uint8_t* bitmap;
} GLYPH;

typedef struct {
    GLYPH glyphs[FONT_DATA_CAPACITY];
    uint8_t HOW_MANY;
} FONT_DATA;

FONT_DATA* GET_FONT_DATA(const char* filepath);

void DESTROY_FONT_DATA(FONT_DATA* font_data);

void DRAW_TEXT(
    char* text,
    FONT_DATA* font,
    uint32_t color,
    uint8_t font_size,
    SDL_Surface* surface,
    SDL_Rect rect,
    int line
);

extern FONT_DATA* DEFAULT_FONT;

void TEXT_INIT();
void TEXT_QUIT();

#endif