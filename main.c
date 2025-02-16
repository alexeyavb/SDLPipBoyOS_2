#include "main.h"
#include "common_defs.h"
#include "common.h"
#include "fps.h"

#include "core_init.h"
#include "ttf_utils.h"
#include "mainmenu.h"

#include <stdbool.h>

extern void draw_fps(void);
extern void draw_main_screen_border(void);
extern void draw_main_menu(void);
#ifndef MAX_STRING_LEN
    #define MAX_STRING_LEN 6
#endif

static char text[MAX_STRING_LEN];

SDL_Rect rect;
SDL_Color color;
static int one_symb_w = 0;
static int one_symb_h = 0;

// main screen defs
SDL_Rect screen_rect;
SDL_Rect main_menu_rect;
SDL_Rect current_rect;

extern void draw_main_menu_header(void);
extern void render_demo_mode(void);
extern void draw_main_menu_items(void);

int main(int argc, char *argv[]){
    SDL_Event event;
    core_init();
    printf("joisticks found %d \n", SDL_NumJoysticks());
    main_menu_init(NULL);
    
    extern int bmp_rain2(void);
    // extern int bmp_rain2(void);

    bmp_rain2();

    while (1) {        
        
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;

        switch(event.type){
            case SDL_KEYDOWN:{
                switch( event.key.keysym.sym ){
                    case SDLK_RIGHT:{
                        extern int active_menu_item;
                        active_menu_item = active_menu_item == (MM_SIZE -1) ? 0 : active_menu_item + 1;
                        break;
                    }
                    case SDLK_LEFT:{
                        extern int active_menu_item;
                        active_menu_item = active_menu_item == 0 ? (MM_SIZE-1) : active_menu_item - 1;
                        break;
                    }
                    default:
                        break;
                }
            }
        }        

        draw_fps();        
        draw_main_screen_border();        
        draw_main_menu_header();
        draw_main_menu_items();
        
        // demo defs
        render_demo_mode();

        SDL_RenderPresent(renderer);
        common_fps_update_and_print();        
    }
    main_menu_deinit();
    core_deinit();
    return 0;
}

void draw_fps(void){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    color.r = 200; color.g = 200; color.b = 30; color.a = 200;

    render_text(renderer, 0, 0, "*", TTF_FontCache[DEF_FONT_IDX], &rect, &color);
    one_symb_w = rect.w;
    one_symb_h = rect.h;

    snprintf(text, MAX_STRING_LEN, "%u", (unsigned int)(time(NULL) % 100000));
    render_text(renderer, DEF_SCREEN_WIDTH - (MAX_STRING_LEN * one_symb_w) - 1, 0, text, TTF_FontCache[DEF_FONT_IDX], &rect, &color);
}

void draw_main_screen_border(void){    
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    screen_rect.x = one_symb_w + 1;
    screen_rect.y = one_symb_h + 1;
    screen_rect.w = DEF_SCREEN_WIDTH - (2*one_symb_w + 1);
    screen_rect.h = DEF_SCREEN_HEIGHT - (2*one_symb_h + 1);
    SDL_RenderDrawRect(renderer, &screen_rect);
}
