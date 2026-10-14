#ifndef LABEL_H
#define LABEL_H

#include "widget.h"
#include "text.h"

typedef struct {
    WIDGET widget;
    uint32_t background_color;

    uint8_t padding_x;
    uint8_t padding_y;

    int scroll_y;
    int max_scroll_y;

    FONT_DATA* font;
    char* text;
    uint16_t font_size;
    uint32_t color;
    
    char* title;
    uint32_t title_color;
    uint32_t title_bacgkround_color;
} LABEL;

void DRAW_LABEL(WIDGET* w,SDL_Surface* surface);

void DESTROY_LABEL(WIDGET* w);

LABEL* CREATE_LABEL(
    int x,int y,
    size_t w,size_t h
);

#endif