#include "widget.h"
#include <stdlib.h> // malloc
#include "SDL2/SDL.h" // SDL_Rect

// WIDGET

bool POINT_IN_RECT(int x,int y,SDL_Rect rect){
    if(x>=rect.x && x<rect.x+rect.w){
        if(y>=rect.y && y<rect.y+rect.h){
            return true;
        }
    }

    return false;
}

SDL_Rect ABSOLUTE_WIDGET_POS(WIDGET* w){
    if(!w) return (SDL_Rect){-1,-1,0,0};
    int x = w->x;
    int y = w->y;

    WIDGET* curr_parent = w->parent;
    while(curr_parent){
        x += curr_parent->x;
        y += curr_parent->y;
        curr_parent = curr_parent->parent;
    }

    return (SDL_Rect){x,y,w->w,w->h};
}

void INIT_WIDGET(
    WIDGET* widget,
    WIDGET_TYPE type,
    int16_t x,int16_t y,
    uint16_t w,uint16_t h,
    void (*DESTROY)(WIDGET*),
    void (*DRAW)(WIDGET*,SDL_Surface*),
    bool (*HANDLE_EVENT)(WIDGET*,SDL_Event*)
){
    if(!widget) return;
    *widget = (WIDGET){
        .type = type,
        .x = x,.y = y,
        .w = w,.h = h,
        .children = (WIDGET_LIST){NULL,NULL},
        .hovered = false,
        .dragging = false,
        .DESTROY = DESTROY,
        .HANDLE_EVENT = HANDLE_EVENT,
        .DRAW = DRAW,
        .parent = NULL,
        .next = NULL,
        .prev = NULL
    };
}

bool APPEND_CHILD(WIDGET* parent,WIDGET* child){
    if(!parent || !child) return false;
    bool appended = WIDGET_LIST_APPEND(&parent->children,child);
    if(appended) child->parent = parent;
    return appended;
}

// WIDGET_LIST

WIDGET_LIST* CREATE_WIDGET_LIST(void){
    WIDGET_LIST* list = (WIDGET_LIST*)malloc(sizeof(WIDGET_LIST));
    if(!list) return NULL;
    list->head = NULL;
    list->tail = NULL;
    return list;
}

void FREE_WIDGET_LIST_WIDGETS(WIDGET_LIST* list){
    if(!list || !list->head) return;
    WIDGET* current = list->head;
    while(current){
        WIDGET* next = current->next;
        if(current->DESTROY){
            if(current==TARGET_WIDGET) TARGET_WIDGET = NULL;
            current->DESTROY(current);
        }

        current = next;
    }
}

void DRAW_WIDGET_LIST_WIDGETS(WIDGET_LIST* list,SDL_Surface* surface){
    if(!list || !list->head) return;
    WIDGET* current = list->head;
    while(current){
        if(current->DRAW) current->DRAW(current,surface);
        DRAW_WIDGET_LIST_WIDGETS(&current->children,surface);
        current = current->next;
    }
}

WIDGET* WIDGET_LIST_HANDLE_EVENT(WIDGET_LIST* list,SDL_Event* e){
    if(!list || !e) return NULL;

    WIDGET* w = list->head;
    while(w){
        WIDGET* handled = WIDGET_LIST_HANDLE_EVENT(&w->children,e);
        if(handled) return handled;

        if(w->HANDLE_EVENT && w->HANDLE_EVENT(w,e)){
            return w;
        }

        w = w->next;
    }

    return NULL;
}

void DESTROY_WIDGET_LIST(WIDGET_LIST* list){
    if(!list) return;

    FREE_WIDGET_LIST_WIDGETS(list);

    free(list);
}

bool WIDGET_LIST_APPEND(WIDGET_LIST* list,WIDGET* widget){
    if(!list || !widget) return false;
    if(!list->head){
        list->head = widget;
    }else{
        list->tail->next = widget;
        widget->prev = list->tail;
    }

    list->tail = widget;
    return true;
}

// WIDGET_MANAGER

WIDGET_MANAGER* CREATE_WIDGET_MANAGER(){
    WIDGET_MANAGER* manager = (WIDGET_MANAGER*)malloc(sizeof(WIDGET_MANAGER));
    if(!manager) return NULL;
    manager->ROOT.head = NULL;
    manager->ROOT.tail = NULL;

    manager->surface = NULL;

    manager->last_hovered = NULL;

    if(!TARGET_ROOT) TARGET_ROOT = &manager->ROOT;
    return manager;
}

void WIDGET_MANAGER_HANDLE_EVENT(WIDGET_MANAGER* manager,SDL_Event* e){
    if(!manager || !e) return;
    WIDGET* handled = WIDGET_LIST_HANDLE_EVENT(&manager->ROOT,e);

    switch(e->type){
        case SDL_MOUSEMOTION:
            WIDGET* last_hovered = manager->last_hovered;
            if(last_hovered!=handled){
                if(last_hovered) last_hovered->hovered = false;
                manager->last_hovered = handled;
            }

            if(manager->dragging_widget){
                WIDGET* dragging = manager->dragging_widget;
                int offset_x = dragging->mouse_offset_x;
                int offset_y = dragging->mouse_offset_y;

                int mx = e->motion.x;
                int my = e->motion.y;

                dragging->x = mx - offset_x;
                dragging->y = my - offset_y;
            }

            break;
        case SDL_MOUSEBUTTONDOWN:
            if(e->button.button==SDL_BUTTON_LEFT){
                if(handled && handled->dragging){
                    manager->dragging_widget = handled;
                }
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if(e->button.button==SDL_BUTTON_LEFT){
                if(manager->dragging_widget){
                    manager->dragging_widget->dragging = false;
                    manager->dragging_widget = NULL;
                }
            }
            break;
        default:
            break;
    }
}

void DESTROY_WIDGET_MANAGER(WIDGET_MANAGER* manager){
    if(!manager) return;
    if(TARGET_ROOT==&manager->ROOT) TARGET_ROOT=NULL;
    FREE_WIDGET_LIST_WIDGETS(&manager->ROOT);
    free(manager);
}

WIDGET_LIST* TARGET_ROOT = NULL;
WIDGET* TARGET_WIDGET = NULL;

void APPEND(WIDGET* widget){
    if(!widget) return;
    if(TARGET_WIDGET){
        APPEND_CHILD(TARGET_WIDGET,widget);
    }else{
        WIDGET_LIST_APPEND(TARGET_ROOT,widget);
    }
}