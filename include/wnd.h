#ifndef WND_H
#define WND_H

#include "widget.h"
#include <stdint.h> // uint32_t

typedef struct {
    WIDGET widget;
    uint32_t background_color;
    uint32_t tab_color;
    char* title;
    uint32_t title_color;

    bool can_drag;
} WND;

void DRAW_WND(WIDGET* w,SDL_Surface* surface);
void DESTROY_WND(WIDGET* w);

WND* CREATE_WND(
    int16_t x,int16_t y,
    uint16_t w,uint16_t h,
    uint32_t background_color
);

#endif