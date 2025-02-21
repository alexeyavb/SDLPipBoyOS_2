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
// SDL_Rect main_menu_rect;
// SDL_Rect current_rect;

extern void draw_main_menu_header(void);
extern void render_demo_mode(void);

extern void draw_main_menu_items(void);
extern void set_next_active_submenu(void);

extern void draw_alpha_line(void);


extern void draw_motorcicle(void); //test


int main(int argc, char *argv[]){
    SDL_Event event;
    core_init();
    main_menu_init(NULL);
    
    extern int bmp_rain2(void);
    // extern int bmp_rain2(void);
    extern void EventThread_Init(void); EventThread_Init();

    // bmp_rain2();
    extern int active_menu_item;
    extern bool expandorcall_cb;
    extern bool expandordown_cb;
    while (1) {        
        
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;

        switch(event.type){
            case SDL_KEYDOWN:{
                switch( event.key.keysym.sym ){
                    case SDLK_RIGHT:{
                        active_menu_item = active_menu_item == (MM_SIZE -1) ? 0 : active_menu_item + 1;
                        break;
                    }
                    case SDLK_LEFT:{
                        active_menu_item = active_menu_item == 0 ? (MM_SIZE-1) : active_menu_item - 1;
                        break;
                    }
                    case SDLK_DOWN:{
                        expandordown_cb = true;
                        // Если в открытом субменю смещаемся вниз
                        // Если в главном меню раскрывываем пункт аналогично VK_RETURN
                        break;
                    }
                    case SDLK_RETURN:{
                        expandorcall_cb = true;
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
        
        // main menu routines
        
        // demo image render
        draw_motorcicle(); 

        draw_main_menu_items();
        set_next_active_submenu();
        extern void expandorcallacction(void); expandorcallacction();
        
        // demo defs       
        render_demo_mode();
        draw_alpha_line();


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

static int w_delta = (int)(DEF_SCREEN_WIDTH);
static int h_delta = (int)(DEF_SCREEN_HEIGHT);
static const int next_rot_frame_delay = 100;
static int next_rot_frame;
static double angle;
static const int no_rotate = 300;
static int nul_angle_delay;
static double zoom_step;

void draw_motorcicle(void){

    SDL_Rect destination;
    SDL_Surface* surface;

    destination.x = (int)(DEF_SCREEN_WIDTH/2) - (int)(DEF_SCREEN_WIDTH/6);
    destination.y = (mainmenufullrect.y * 2) + mainmenufullrect.h;
    destination.h = DEF_SCREEN_HEIGHT - mainmenufullrect.y * 2 - destination.y;
    destination.w = DEF_SCREEN_WIDTH - mainmenufullrect.x - destination.x;
    
    // angle = 360.0f;
    zoom_step = 6.0f;
    // zoom_step = (360>=angle && 1.0f >= zoom_step ? zoom_step+0.1f : 0.5);        

    if(SDL_GetTicks() >= next_rot_frame && nul_angle_delay >= no_rotate){        
        angle = ( 360.0f >= angle ?  angle + 40 : 0.0f);
        next_rot_frame = SDL_GetTicks() + next_rot_frame_delay;
    }

    surface = IMG_Load("../data/jpg/motorcicle/scout.jpg\0"); 
    
    SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_BLEND);    
    SDL_Surface* rotated = rotozoomSurface(surface, angle, zoom_step, 0);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, rotated); 
    SDL_SetTextureAlphaMod(texture, 0x5d);        
    SDL_FreeSurface(surface); SDL_FreeSurface(rotated);
    SDL_RenderCopy(renderer, texture, NULL, &destination);        
    SDL_DestroyTexture(texture);

    nul_angle_delay = 360.0f == angle && nul_angle_delay >= no_rotate ? 0 : nul_angle_delay + 1;

    // double angle = -(atan2(destination.x + destination.w/2 , destination.y + destination.h/2)) * (180/M_PI) - 90.0f;

}