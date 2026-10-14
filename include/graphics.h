#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h> // uint32_t

typedef struct SDL_Surface SDL_Surface;
typedef struct SDL_Rect SDL_Rect;

void clear(SDL_Surface* surface,uint32_t color);
void fill_rect(SDL_Surface* surface,SDL_Rect rect,uint32_t color);

#endif