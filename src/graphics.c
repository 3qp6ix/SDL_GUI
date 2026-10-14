#include "graphics.h"

#include "SDL2/SDL.h"

void clear(SDL_Surface* surface,uint32_t color){
    if(!surface) return;
    uint32_t* buffer = (uint32_t*)surface->pixels;
    for(size_t i=0;i<surface->w*surface->h;i++){
        buffer[i] = color;
    }
}

void fill_rect(SDL_Surface* surface,SDL_Rect rect,uint32_t color){
    if(!surface) return;

    uint32_t* buffer = (uint32_t*)surface->pixels;

    int x1 = rect.x;
    int y1 = rect.y;
    int x2 = x1 + rect.w;
    int y2 = y1 + rect.h;

    if(x1<0 && x2<0) return;
    if(y1<0 && y2<0) return;
    if(x1>surface->w && x2>surface->w) return;
    if(y1>surface->h && y2>surface->h) return;

    if(x1<0) x1=0;
    if(y1<0) y1=0;
    if(x2>surface->w) x2=surface->w;
    if(y2>surface->h) y2=surface->h;

    for(int y=y1;y<y2;y++){
        for(int x=x1;x<x2;x++){
            buffer[y*surface->w+x] = color;
        }
    }
}