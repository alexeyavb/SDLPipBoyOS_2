#include "common_defs.h"
#include "common.h"
#include "alpha_line.h"

#define DEF_ALPHA_LINE_HEIGHT (int)(DEF_SCREEN_HEIGHT/3)
#define DEF_ALINE_DELAY 500
#define DEF_ALPHA_LINE_SCALE 1.1f

#define DEF_ALPHA_LINE_CLR_R 5
#define DEF_ALPHA_LINE_CLR_G 200
#define DEF_ALPHA_LINE_CLR_B 200
#define DEF_ALPHA_LINE_ACHANNEL 200

extern SDL_Window *window;
extern SDL_Renderer *renderer;

static SDL_Rect aLineRect;
static int x = 0, y = 0;

void draw_alpha_line(void){
    aLineRect.x = x;
    aLineRect.y = y;
    aLineRect.w = DEF_SCREEN_WIDTH;
    aLineRect.h = DEF_ALPHA_LINE_HEIGHT;
    
    SDL_SetRenderDrawColor(renderer, DEF_ALPHA_LINE_CLR_R, DEF_ALPHA_LINE_CLR_G, DEF_ALPHA_LINE_CLR_B, DEF_ALPHA_LINE_ACHANNEL);
    
    

}
