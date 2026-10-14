#include "SDL2/SDL.h" // SDL2
#include "graphics.h" // clear()
#include "gui.h"

#include <stdbool.h> // bool

size_t W_WIDTH = 800;
size_t W_HEIGHT = 600;

void draw_chessboard(WIDGET* w,SDL_Surface* surface){
    if(!w || !surface) return;


    clear(surface,0xFFFF0000);
}

int main(int argc,char** argv){
    if(SDL_Init(SDL_INIT_VIDEO)!=0) return 1;

    SDL_Window* main_window = SDL_CreateWindow(
        "Window",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        W_WIDTH,W_HEIGHT,
        SDL_WINDOW_RESIZABLE
    );

    if(!main_window){
        SDL_Quit();
        return 1;
    }

    DEFAULT_FONT = GET_FONT_DATA("./assets/default_font.txt");

    SDL_Surface* main_surface = SDL_GetWindowSurface(main_window);

    WIDGET_MANAGER* widget_manager = CREATE_WIDGET_MANAGER();
    widget_manager->surface = main_surface;

    WND* wnd_a = CREATE_WND(10,10,400,300,0xFF252936);
    wnd_a->can_drag = true;
    wnd_a->tab_color = 0xFF465675;
    wnd_a->title = "WINDOW A";
    
    TARGET_WIDGET = (WIDGET*)wnd_a;
    WND* wnd_b = CREATE_WND(5,30,200,100,0xFFE8ECF2);
    wnd_b->tab_color = 0xFFCBD5E1;
    wnd_b->title = "WINDOW B";
    wnd_b->title_color = 0xFF1E293B;

    LABEL* label = CREATE_LABEL(210,30,180,100);
    label->title = "Hello world!";
    label->scroll_y = 0;
    label->max_scroll_y = 5;
    label->text = 
    "  #### LABEL ####  \n\n"
    "Hello, stranger.\n"
    "How are you?\n\n"
    "Good luck! :) :]\n\n"
    "ABCDEFGHIJKLMN\nOPQRSTUVWXYZ\n1234567890\n\n"
    "  - 3QP6IX";

    bool running = 1;
    SDL_Event e;
    while(running){
        while(SDL_PollEvent(&e)){
            WIDGET_MANAGER_HANDLE_EVENT(widget_manager,&e);
            switch(e.type){
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_WINDOWEVENT:
                    if(e.window.event==SDL_WINDOWEVENT_RESIZED){
                        W_WIDTH = e.window.data1;
                        W_HEIGHT = e.window.data2;
                    }
                    break;
                default:
                break;
            }
        }

        clear(main_surface,0xFFFFFFFF);

        DRAW_WIDGET_LIST_WIDGETS(TARGET_ROOT,main_surface);

        SDL_UpdateWindowSurface(main_window);
    }

    DESTROY_FONT_DATA(DEFAULT_FONT);
    DESTROY_WIDGET_MANAGER(widget_manager);
    SDL_DestroyWindow(main_window);
    SDL_Quit();
    return 0;
}