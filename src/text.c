#include "text.h"

#include <stdio.h> // FILE
#include <stdlib.h> // malloc
#include <string.h> // strcmp
#include <ctype.h> // to_upper

#include "graphics.h" // fill_rect

FONT_DATA* GET_FONT_DATA(const char* filepath){
    if(!filepath) return NULL;
    FILE* file = fopen(filepath,"rb");
    if(!file) return NULL;

    FONT_DATA* data = (FONT_DATA*)calloc(1,sizeof(FONT_DATA));
    if(!data){
        fclose(file);
        return NULL;
    }

    fscanf(file,"%hhu",&data->HOW_MANY);

    char token[15];
    for(size_t i=0;i<data->HOW_MANY;i++){
        fscanf(file,"%14s",token);
        if(strcmp(token,"SPACE")==0) token[0] = ' ';

        GLYPH* curr = &data->glyphs[token[0]];
        fscanf(
            file,
            "%hhu,%hhu,%hhu",
            &curr->width,
            &curr->height,
            &curr->increment
        );

        uint8_t* bitmap = calloc(curr->height,sizeof(uint8_t));
        if(!bitmap){
            free(data);
            fclose(file);
            return NULL;
        }

        char row[30];
        for(size_t y=0;y<curr->height;y++){
            fscanf(file,"%29s",row);
            uint8_t bits = 0;

            for(size_t x=0;x<curr->width;x++){
                bits <<= 1;
                if(row[x]=='1'){
                    bits |= 1;
                }
            }

            bitmap[y] = bits;
        }

        curr->bitmap = bitmap;
    }

    fclose(file);
    return data;
}

void DESTROY_FONT_DATA(FONT_DATA* font_data){
    if(!font_data) return;
    for(size_t i=0;i<FONT_DATA_CAPACITY;i++){
        GLYPH* curr = &font_data->glyphs[i];
        if(curr->width==0) continue;
        free(curr->bitmap);
    }
    free(font_data);
}

void DRAW_TEXT(
    char* text,
    FONT_DATA* font,
    uint32_t color,
    uint8_t font_size,
    SDL_Surface* surface,
    SDL_Rect rect,
    int line
){
    if(!text || !font || !surface) return;

    int x2 = rect.x + rect.w;
    int y2 = rect.y + rect.h;

    uint8_t scale = font_size/8;
    int spacing = scale * 2;

    int cursor_x = rect.x;
    int cursor_y = rect.y - (font_size+spacing) * line;

    while(*text!='\0'){
        GLYPH* curr = &font->glyphs[toupper(*text)];
        if(!curr){text++;continue;}

        if(*text=='\n'){
            cursor_x = rect.x;
            cursor_y += font_size + spacing;
        }

        if(cursor_x+curr->width*scale>x2){
            cursor_x = rect.x;
            cursor_y += font_size + spacing;
        }

        for(uint8_t y=0;y<curr->height;y++){
            if(cursor_y+y*scale<rect.y) continue;
            if(cursor_y+y*scale>=y2) return;
            for(uint8_t x=0;x<curr->width;x++){
                uint8_t bit = (curr->bitmap[y] >> curr->width - x - 1) & 0b1;

                SDL_Rect cell = (SDL_Rect){
                    cursor_x+x*scale,
                    cursor_y+y*scale,
                    scale,scale
                };

                if(!bit) continue;
                fill_rect(surface,cell,color);
            }
        }

        cursor_x += curr->increment * scale;
        text++;
    }
}

FONT_DATA* DEFAULT_FONT = NULL;