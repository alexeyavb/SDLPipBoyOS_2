#include "common_defs.h"
#include "common.h"
#include "alpha_line.h"

#define DEF_ALPHA_LINE_HEIGHT (int)(DEF_SCREEN_HEIGHT/3)
#define DEF_ALINE_DELAY 500
#define DEF_ALPHA_LINE_SCALE 1.1f

// #25292e
#define DEF_ALPHA_LINE_CLR_R 0x25
#define DEF_ALPHA_LINE_CLR_G 0x29
#define DEF_ALPHA_LINE_CLR_B 0x2e
#define DEF_ALPHA_LINE_ACHANNEL 0xfa
#define DEF_RECT_HEIGHT  (int)(((int) DEF_SCREEN_HEIGHT/3)/6)
extern SDL_Window *window;
extern SDL_Renderer *renderer;

// static int x = 0, y = 0;

void draw_alpha_line(void){    
    SDL_SetRenderDrawColor(renderer, DEF_ALPHA_LINE_CLR_R, DEF_ALPHA_LINE_CLR_G, DEF_ALPHA_LINE_CLR_B, DEF_ALPHA_LINE_ACHANNEL);
    SDL_Rect LineRect = {0,0, DEF_SCREEN_WIDTH, DEF_RECT_HEIGHT}; // x,y, w,hB
    SDL_RenderFillRect(renderer, &LineRect);
}
