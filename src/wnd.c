#include "wnd.h"
#include "SDL2/SDL.h" // SDL_Rect
#include <stdlib.h> // malloc
#include "graphics.h" // fill_rect()
#include "text.h"

bool WND_HANDLE_EVENT(WIDGET* w,SDL_Event* e){
    if(!w || !e) return false;

    SDL_Rect abs = ABSOLUTE_WIDGET_POS(w);

    WND* wnd = (WND*)w;

    switch(e->type){
        case SDL_MOUSEMOTION:
            int mx = e->motion.x;
            int my = e->motion.y;

            SDL_Rect rect = (SDL_Rect){abs.x,abs.y,w->w,w->h+WINDOW_TAB_HEIGHT};
            bool IN_RECT = POINT_IN_RECT(mx,my,rect);

            if(IN_RECT){
                if(!w->hovered) w->hovered = true;
                return true;
            }

            break;
        case SDL_MOUSEBUTTONDOWN:
            mx = e->button.x;
            my = e->button.y;
            if(e->button.button == SDL_BUTTON_LEFT){
                bool IN_TAB = POINT_IN_RECT(mx,my,(SDL_Rect){abs.x,abs.y,w->w,WINDOW_TAB_HEIGHT});
                
                if(IN_TAB && wnd->can_drag){
                    if(!w->dragging){
                        w->mouse_offset_x = mx - w->x;
                        w->mouse_offset_y = my - w->y;
                        w->dragging = true;
                    }
                    return true;
                }
            }
            break;
        default:
            return false;
            break;
    }

    return false;
}

void DRAW_WND(WIDGET* w,SDL_Surface* surface){
    if(!w || !surface) return;

    WND* wnd = (WND*)w;

    SDL_Rect abs = ABSOLUTE_WIDGET_POS(w);

    fill_rect(
        surface,
        (SDL_Rect){abs.x,abs.y,w->w,WINDOW_TAB_HEIGHT},
        w->hovered? 0xFF245263 : wnd->tab_color
    );

    DRAW_TEXT(
        wnd->title,
        DEFAULT_FONT,
        w->hovered? 0xFFFFFFFF : wnd->title_color,
        8,surface,(SDL_Rect){abs.x+5,abs.y+(WINDOW_TAB_HEIGHT-8)/2,w->w-10,8},
        0
    );

    fill_rect(
        surface,
        (SDL_Rect){abs.x,abs.y+WINDOW_TAB_HEIGHT,w->w,w->h},
        wnd->background_color
    );
}

void DESTROY_WND(WIDGET* w){
    if(!w) return;
    WND* wnd = (WND*)w;
    free(wnd);
}

WND* CREATE_WND(
    int16_t x,int16_t y,
    uint16_t w,uint16_t h,
    uint32_t background_color
){
    if(!TARGET_ROOT) return NULL;
    
    WND* new_wnd = (WND*)malloc(sizeof(WND));
    if(!new_wnd) return NULL;

    INIT_WIDGET(
        &new_wnd->widget,
        WND_TYPE,
        x,y,w,h,
        DESTROY_WND,
        DRAW_WND,
        WND_HANDLE_EVENT
    );

    new_wnd->background_color = background_color;
    new_wnd->tab_color = DEFAULT_WINDOW_TAB_COLOR;
    new_wnd->title = "WINDOW";
    new_wnd->title_color = 0xFFFFFFFF;
    new_wnd->can_drag = false;

    APPEND((WIDGET*)new_wnd);
    return new_wnd;
}