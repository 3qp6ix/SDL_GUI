#include "label.h"
#include <stdlib.h> // malloc
#include "SDL2/SDL.h" // SDL_Surface,SDL_Rect
#include "graphics.h" // fill_rect()

void DESTROY_LABEL(WIDGET* w){
    if(!w) return;
    LABEL* label = (LABEL*)w;
    free(label);
};

bool LABEL_HANDLE_EVENT(WIDGET* w,SDL_Event* e){
    if(!w || !e) return false;

    LABEL* label = (LABEL*)w;

    switch(e->type){
        case SDL_MOUSEMOTION:
            int mx = e->motion.x;
            int my = e->motion.y;

            SDL_Rect abs = ABSOLUTE_WIDGET_POS(w);

            bool IN_RECT = POINT_IN_RECT(mx,my,(SDL_Rect){abs.x,abs.y,w->w,w->h});

            if(IN_RECT){
                w->hovered = true;
                return true;
            }

            break;
        case SDL_MOUSEWHEEL:
            if(!w->hovered) return false;

            if(e->wheel.y<0){

                if(label->scroll_y<label->max_scroll_y){
                    label->scroll_y += 1;
                }

            }else if(e->wheel.y>0){

                if(label->scroll_y>0){ 
                    label->scroll_y -= 1;
                }

            }

            return true;
            break;
        default:
            break;
    }

    return false;
}

void DRAW_LABEL(WIDGET* w,SDL_Surface* surface){
    if(!w || !surface) return;
    
    SDL_Rect abs = ABSOLUTE_WIDGET_POS(w);

    LABEL* label = (LABEL*)w;

    fill_rect(
        surface,
        (SDL_Rect){abs.x,abs.y,w->w,w->h},
        label->background_color
    );

    uint8_t title_height = 16;
    int padding = 0;

    if(label->title){
        padding = title_height;

        fill_rect(
            surface,
            (SDL_Rect){abs.x,abs.y,w->w,title_height},
            label->title_bacgkround_color
        );

        DRAW_TEXT(
            label->title,DEFAULT_FONT,
            label->title_color,8,
            surface,
            (SDL_Rect){abs.x+2,abs.y+4,w->w-4,8},
            0
        );
    }

    DRAW_TEXT(
        label->text,label->font,
        label->color,label->font_size,
        surface,
        (SDL_Rect){
            abs.x+label->padding_x,
            abs.y+padding+label->padding_y,
            w->w-label->padding_x*2,
            w->h-label->padding_y*2-padding
        },label->scroll_y
    );
}

LABEL* CREATE_LABEL(
    int x,int y,
    size_t w,size_t h
){
    if(!TARGET_ROOT && !TARGET_WIDGET) return NULL;

    LABEL* new_label = (LABEL*)malloc(sizeof(LABEL));
    if(!new_label) return NULL;

    INIT_WIDGET(
        &new_label->widget,
        LABEL_TYPE,
        x,y,w,h,
        DESTROY_LABEL,
        DRAW_LABEL,
        LABEL_HANDLE_EVENT
    );

    new_label->background_color = 0xFF211609;
    new_label->padding_x = 5;
    new_label->padding_y = 5;
    new_label->font = DEFAULT_FONT;
    new_label->text = NULL;
    new_label->font_size = 8;
    new_label->scroll_y = 0;
    new_label->max_scroll_y = 0;
    new_label->color = 0xFFFFB000;
    new_label->title = NULL;
    new_label->title_color = 0xFFFFCC66;
    new_label->title_bacgkround_color = 0xFF754400;

    APPEND((WIDGET*)new_label);

    return new_label;
};