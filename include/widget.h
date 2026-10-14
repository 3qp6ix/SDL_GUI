#ifndef WIDGET_H
#define WIDGET_H

#define WINDOW_TAB_HEIGHT 25
#define DEFAULT_WINDOW_TAB_COLOR 0xFF5566FF

#include <stdint.h> // int16_t, uint16_t
#include <stdbool.h> // bool

typedef struct SDL_Surface SDL_Surface;
typedef struct SDL_Rect SDL_Rect;
typedef union SDL_Event SDL_Event;

typedef enum {
    WND_TYPE,
    LABEL_TYPE

} WIDGET_TYPE;

typedef struct WIDGET WIDGET;

typedef struct {
    WIDGET* head;
    WIDGET* tail;
} WIDGET_LIST;

typedef struct WIDGET {
    WIDGET_TYPE type;
    int16_t x,y;
    uint16_t w,h;

    WIDGET_LIST children;

    bool hovered;

    bool dragging;
    int mouse_offset_x;
    int mouse_offset_y;

    void (*DESTROY)(WIDGET*);
    void (*DRAW)(WIDGET*,SDL_Surface*);
    bool (*HANDLE_EVENT)(WIDGET*,SDL_Event* e);

    WIDGET* parent;
    WIDGET* next;
    WIDGET* prev;
} WIDGET;

// WIDGET

bool POINT_IN_RECT(int x,int y,SDL_Rect rect);

SDL_Rect ABSOLUTE_WIDGET_POS(WIDGET* w);

void INIT_WIDGET(
    WIDGET* widget,
    WIDGET_TYPE type,
    int16_t x,int16_t y,
    uint16_t w,uint16_t h,
    void (*DESTROY)(WIDGET*),
    void (*DRAW)(WIDGET*,SDL_Surface*),
    bool (*HANDLE_EVENT)(WIDGET*,SDL_Event*)
);

bool APPEND_CHILD(WIDGET* parent,WIDGET* child);

// WIDGET_LIST

WIDGET_LIST* CREATE_WIDGET_LIST(void);
void FREE_WIDGET_LIST_WIDGETS(WIDGET_LIST* list);
void DRAW_WIDGET_LIST_WIDGETS(WIDGET_LIST* list,SDL_Surface* surface);
WIDGET* WIDGET_LIST_HANDLE_EVENT(WIDGET_LIST* list,SDL_Event* e);
void DESTROY_WIDGET_LIST(WIDGET_LIST* list);
bool WIDGET_LIST_APPEND(WIDGET_LIST* list,WIDGET* widget);

// WIDGET_MANAGER

typedef struct {
    WIDGET_LIST ROOT;
    size_t* W_WIDTH;
    size_t* W_HEIGHT;
    SDL_Surface* surface;
    WIDGET* last_hovered;
    WIDGET* dragging_widget;
} WIDGET_MANAGER;

WIDGET_MANAGER* CREATE_WIDGET_MANAGER();
void WIDGET_MANAGER_HANDLE_EVENT(WIDGET_MANAGER* manager,SDL_Event* e);
void DESTROY_WIDGET_MANAGER(WIDGET_MANAGER* manager);

extern WIDGET_LIST* TARGET_ROOT;
extern WIDGET* TARGET_WIDGET;

void APPEND(WIDGET* widget);

#endif