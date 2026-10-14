#include "text.h"

#include "wnd.h"
#include "label.h"

void GUI_INIT(){
    TEXT_INIT();
}

void GUI_EXIT(){
    DESTROY_FONT_DATA(DEFAULT_FONT);
}