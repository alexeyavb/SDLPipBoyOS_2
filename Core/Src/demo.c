#include "common.h"
#include "common_defs.h"
#include "demo.h"
#include "ttf_utils.h"
// demo defs
SDL_Rect dmo_rect;
SDL_Color dmo_color;

static int next_time = 0;
static int current_demo_alpha = 0;
static bool is_directly = true;
static const int time_delay = 6;
static const int min_alpha = 100;
static const int max_alpha = 254;
static const char* demo_mode_text = "** DEMO MODE **";

SDL_Rect current_rect; // from main

// end demo defs

void render_demo_mode(void){
    current_rect.y = 44;
    
    dmo_color.a = current_demo_alpha;
    dmo_color.r = 0;
    dmo_color.g = 254;
    dmo_color.b = 0;
    int current_time = SDL_GetTicks();
    // get width of one symbol
    render_text(renderer, DEF_SCREEN_WIDTH/2, current_rect.y + current_rect.h + 4, " ", TTF_FontCache[DEF_FONT_24_IDX], &dmo_rect, &dmo_color);
    render_text(renderer, (int)((DEF_SCREEN_WIDTH - (strlen(demo_mode_text) * dmo_rect.w)) /2), current_rect.y + current_rect.h + 4, demo_mode_text, TTF_FontCache[DEF_FONT_24_IDX], &dmo_rect, &dmo_color);
    if(current_time >= next_time){
        (is_directly) ? (current_demo_alpha+=2) : (current_demo_alpha-=2);

        if( is_directly && (current_demo_alpha >= max_alpha)){
            is_directly = !is_directly;
        }

        if(!is_directly && (current_demo_alpha <= min_alpha)){
            is_directly = !is_directly;
        }

        next_time = current_time + time_delay;
    } 
}